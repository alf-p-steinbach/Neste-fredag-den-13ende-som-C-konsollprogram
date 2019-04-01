#pragma once        // Source encoding: UTF-8 with BOM (π is a lowercase Greek "pi").
#include <my/Date.hpp>
#include <my/string-building.hpp>
#include <cppx-core/_all_.hpp>          // https://github.com/alf-p-steinbach/cppx-core

namespace app
{
    $use_cppx( fail, C_str, n_items_in );
    $use_std( cout, endl, vector, runtime_error, string, string_view );
    using namespace std::literals;

    namespace norwegian
    {
        $use_cppx( Raw_array_ );
        $use_std( string, string_view );
        using namespace my::string_building;

        constexpr inline auto month_name( const int i )
            -> string_view
        {
            constexpr Raw_array_<string_view> the_names =
            {
                "januar", "februar", "mars", "april", "mai", "juni", "juli", 
                "august", "september", "oktober", "november", "desember"
            };
            
            return the_names[i];
        }
        
        inline auto daymonth_string_from( const my::Date& date )
            -> string
        { return ""s << date.day() << ". " << month_name( date.month() - 1 ); }
    }  // namespace norwegian
    
    inline void report_result_for( const my::Date& start_date )
    {
        const int friday_number = 6;
        my::Date date = start_date;
        int n_days = 0;
        do
        {
            date.set_day( date.day() + 1 );
            ++n_days;
        } while( date.weekday() != friday_number or date.day() != 13 );

        cout    << n_days << " dager etter "
                << norwegian::daymonth_string_from( start_date )
                << " " << start_date.year()
                << " kommer fredag " << norwegian::daymonth_string_from( date )
                << "... :-o"
                << endl;
    }

    inline auto help_text()
        -> C_str
    {
        static const C_str the_raw_text =
            #include "quickhelp.literal.txt"
            ;
       return the_raw_text + 1;         // Skip initial newline.
    }

    inline void run_with_args( const vector<string>& args )
    {
        struct Oh
        {
            $noreturn
            static void just_throw_as_exit_message( const string& caption )
            {
                fail( caption + "\n\n" + help_text() );
            };
        };
    
        switch( n_items_in( args ) )
        {
            case 0:
            {
                report_result_for( my::Date() );
                break;
            }
            case 1:     
            {
                const auto& arg = args.front();
                if( arg == "/?" or arg == "?" or arg == "--help" )
                {
                    Oh::just_throw_as_exit_message( "Hurtighjelp:" );
                }
                report_result_for( my::Date( args.front() ) );
                break;
            }
            default:
            {
                Oh::just_throw_as_exit_message( "For mange argumenter." );
            }
        }
    }
}  // namespace app
