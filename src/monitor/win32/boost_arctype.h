#ifndef BOOST_ARCTYPE_H
#define BOOST_ARCTYPE_H

#if 0

#include <boost/archive/xml_wiarchive.hpp>
#include <boost/archive/xml_woarchive.hpp>

typedef boost::archive::xml_wiarchive barc_i; // Boost ARChive Input
typedef boost::archive::xml_woarchive barc_o; // Boost ARChive Output
#else
#include <boost/archive/text_wiarchive.hpp>
#include <boost/archive/text_woarchive.hpp>

typedef boost::archive::text_wiarchive barc_i; // Boost ARChive Input
typedef boost::archive::text_woarchive barc_o; // Boost ARChive Output

#endif

#endif
