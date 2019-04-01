#pragma once    // Source encoding: UTF-8 with BOM (π is a lowercase Greek "pi").
#include <cppx-core/_all_.hpp>

namespace my
{
    $use_std( string, move, ostringstream );
    
    // Minimal-code string building. Very very un-optimized. Just convenient.
    inline namespace string_building
    {
        using namespace std::literals;

        template< class Value >
        auto operator<<( string& s, const Value& v )
            -> string&
        {
            ostringstream stream;
            stream << v or $fail( "Failed to convert argument to string." );
            s += stream.str();
            return s;
        }
        
        template< class Value >
        auto operator<<( string&& s, const Value& v )
            -> string&&
        { return move( s << v ); }
    }  // namespace string_building
    
}  // namespace my
