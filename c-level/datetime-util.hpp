#pragma once    // Source encoding: UTF-8 with BOM (π is a lowercase Greek "pi").
#include <cppx-core/_all_.hpp>          // https://github.com/alf-p-steinbach/cppx-core

namespace c
{
    $use_cppx( hopefully, is_in, Range, quoted );
    $use_std( getline, istringstream, string, string_view, to_string );
    using namespace std::literals;

    using Ticks_value           = ::clock_t;
    using Linear_time_value     = ::time_t;
    using Datetime_value        = ::tm;

    namespace linear_time
    {
        constexpr Linear_time_value epoch = {};

        inline auto since_epoch()       // Epoch is e.g. 00:00 1. january 1970.
            -> Linear_time_value
        { return ::time( nullptr ); }
    }  // namespace linear_time

    template< class Specialized_datetime >
    struct Datetime_
    {
        // Provided by Specialized_datetime, which is either UTC or local time:
        //
        // static constexpr bool is_utc;
        //
        // static auto from( const Linear_time_value time )
        //     -> Datetime_value;
        // 
        // static auto normalize( Datetime_value& dt )
        //     -> Linear_time_value;

        static auto compare_date_parts( const Datetime_value& a, const Datetime_value& b )
            -> int
        {
            if( const int d = a.tm_year - b.tm_year )   { return d; }
            if( const int d = a.tm_mon - b.tm_mon )     { return d; }
            return a.tm_mday - b.tm_mday;
        }

        static auto epoch()
            -> const Datetime_value&
        {
            static const Datetime_value the_epoch =
                Specialized_datetime::from( linear_time::epoch );

            return the_epoch;
        }

        static auto now()
            -> Datetime_value
        { return Specialized_datetime::from( linear_time::since_epoch() ); }

        static inline auto from( const string_view& iso_spec )      // Defined below.
            -> Datetime_value;
    };

    template< class Specialized_datetime >
    inline auto Datetime_<Specialized_datetime>::from(
        const string_view& iso_spec
        ) -> Datetime_value
    {
        const char          delimiter   = '-';
        const int           first_year  = epoch().tm_year + 1900;

        Datetime_value         result = {};
        istringstream       stream(( string( iso_spec ) ));     // ← Most vexing parse.
        string              item;

        try
        {
            auto& year = result.tm_year;
            stream >> year
                or $fail( "Feil i årstallsangivelsen." );
            hopefully( year >= first_year )
                or $fail( "Mitt maskineri håndterer ikke årstall før "
                    + to_string( first_year ) + "." );
            year -= 1900;
            
            hopefully( stream.get() == delimiter )
                or $fail( "Skilletegnet etter årstallet må være et bindestrek." );

            auto& month = result.tm_mon;
            stream >> month
                or $fail( "Feil i månedsangivelsen." );
            hopefully( is_in( Range( 1, 12 ), month ) )
                or $fail( "Månedsnummeret må være i området 1 til og med 12." );
            --month;

            if( stream.eof() )
            {
                result.tm_mday = 1;
            }
            else
            {
                hopefully( stream.get() == delimiter )
                    or $fail( "Skilletegnet etter måneden må være en bindestrek." );

                auto& day = result.tm_mday;
                stream >> day
                    or $fail( "Feil i dagsangivelsen." );
                hopefully( is_in( Range( 1, 31 ), day ) )
                    or $fail( "Dagsnummeret må være i området 1 til og med 31." );
                // No adjustment, range of tm_mday is 1 through 31, i.e. 1-based.
            }

            hopefully( stream.eof() )
                or $fail( "Ekstra tegn etter en"
                    " ellers tilsynelatende OK datoangivelse." );

            result.tm_isdst = -1; // Don't know if daylight saving time.
            const Datetime_value original = result;
            const Linear_time_value linear_time = Specialized_datetime::normalize( result );
            hopefully( linear_time != -1 and compare_date_parts( original, result ) == 0 )
                or $fail( "En ugyldig "s + (Specialized_datetime::is_utc? "UCT" : "lokal")
                    + " dato ble angitt (angivelsen var ellers OK)." );
        }
        catch( ... )
        {
            $fail( quoted( iso_spec ) + " er ikke en gyldig datoangivelse." );
        }
        return result;
    }

    struct Local_time:
        Datetime_<Local_time>
    {
        static constexpr bool is_utc = false;
        using Datetime_<Local_time>::from;

        static auto from( const Linear_time_value time )
            -> Datetime_value
        { return *::localtime( &time ); }

        static auto normalize( Datetime_value& dt )
            -> Linear_time_value
        { return ::mktime( &dt ); }
    };

    struct Utc:
        Datetime_<Utc>
    {
        static constexpr bool is_utc = true;
        using Datetime_<Utc>::from;

        static auto from( const Linear_time_value time )
            -> Datetime_value
        { return *::gmtime( &time ); }

        static auto normalize( Datetime_value& dt )
            -> Linear_time_value
        {
            #ifdef _WIN32
                return ::_mkgmtime( &dt );      // Windows
            #else
                return ::timegm( &dt );         // Posix
            #endif
        }
    };
    
}  // namespace c
