#pragma once        // Source encoding: UTF-8 with BOM (π is a lowercase Greek "pi").
#include <c-level/datetime-util.hpp>
#include <my/string-building.hpp>
#include <cppx-core/_all_.hpp>          // https://github.com/alf-p-steinbach/cppx-core

namespace my
{
    $use_cppx( hopefully );
    $use_std( string_view, to_string );

    class Date
    {
        c::Datetime_value   m_local_datetime;

    public:
        // 1-based date item values. For weekdays Sunday = 1.
        auto year() const       -> int  { return 1900 + m_local_datetime.tm_year; }
        auto month() const      -> int  { return 1 + m_local_datetime.tm_mon; }
        auto day() const        -> int  { return 0 + m_local_datetime.tm_mday; }
        auto weekday() const    -> int  { return 1 + m_local_datetime.tm_wday; }

        // Note: will intentionally accept out-of-range day numbers and auto-adjust.
        void set_day( const int day_number )
        {
            c::Datetime_value   date = m_local_datetime;

            date.tm_mday = day_number;
            const c::Linear_time_value linear_time = c::Local_time::normalize( date );
            hopefully( linear_time != -1 )
                or $fail( "Dagsnummer "s << day_number << " ga en ugyldig dato." );
            m_local_datetime = date;
        }

        Date():
            m_local_datetime( c::Local_time::now() )
        {}

        Date( const string_view& iso_spec ):
            m_local_datetime( c::Local_time::from( iso_spec ) )
        {}
    };
};  // namespace my
