lib_LTLIBRARIES = libartc42.la libcomper.la libenigma.la libkosinski.la libkosplus.la liblzkn1.la libnemesis.la librocket.la libsaxman.la libsnkrle.la

noinst_LIBRARIES = libstaticanalyzer.a

libartc42_la_SOURCES = artc42.cc

libcomper_la_SOURCES = comper.cc

libenigma_la_SOURCES = enigma.cc

libkosinski_la_SOURCES = kosinski.cc

libkosplus_la_SOURCES = kosplus.cc

liblzkn1_la_SOURCES = lzkn1.cc

libnemesis_la_SOURCES = nemesis.cc

librocket_la_SOURCES = rocket.cc

libsaxman_la_SOURCES = saxman.cc

libsnkrle_la_SOURCES = snkrle.cc

libstaticanalyzer_a_SOURCES = \
    basic_decoder.cc \
    bigendian_io.cc \
    bigendian_io.cc \
    bitstream.cc \
    ignore_unused_variable_warning.cc \
    lzss.cc \
    moduled_adaptor.cc

AM_CPPFLAGS = -I$(abs_top_srcdir)/include

AM_LDFLAGS = -no-undefined
