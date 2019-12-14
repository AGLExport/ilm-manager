#-------------------------------------------------------------------------
# 実行ファイル作成の例
#-------------------------------------------------------------------------

# この行は触らない
AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

#ビルドする実行ファイルの指定(exsample)
bin_PROGRAMS = ilmmanager


# ビルドする実行ファイル(exsample)を構成するソースファイルの指定
ilmmanager_SOURCES = \
		CWindowManager.cpp \
		CILMResource.cpp \
		CILMConfig.cpp \
		ilm-manager.cpp 



# ビルドする実行ファイル(exsample)のリンカに渡すライブラリの指定を行う。
# @XXX_LIBS@のXXXは、configure.acでPKG_CHECK_MODULESで指定したパッケージの名前を指定する。
# この例はGLIBとZLIBを指定している。
ilmmanager_LDADD = \
		-lrt -lpthread \
		@JSONCPP_LIBS@ \
		@ILMCONTROL_LIBS@ \
		@ILMCOMMON_LIBS@ \
		@ILMINPUT_LIBS@ 


# ビルドする実行ファイル(exsample)のコンンパイラオプション(c)の指定を行う。
# @XXX_CFLAGS@のXXXは、configure.acでPKG_CHECK_MODULESで指定したパッケージの名前を指定する。
# この例はGLIBとZLIBを指定している。
ilmmanager_CFLAGS = \
		-I$(top_srcdir)/include \
		-g \
		@JSONCPP_CFLAGS@ \
		@ILMCONTROL_CFLAGS@ \
		@ILMCOMMON_CFLAGS@ \
		@ILMINPUT_CFLAGS@ 

# ビルドする実行ファイル(exsample)のコンンパイラオプション(c++)の指定を行う。
# @XXX_CXXFLAGS@のXXXは、configure.acでPKG_CHECK_MODULESで指定したパッケージの名前を指定する。
# CのライブラリはCXXFLAGSがないので、渡すパラメタはCFLAGSを指定する。
# この例はGLIBとZLIBを指定している。
ilmmanager_CXXFLAGS = \
		-I$(top_srcdir)/include \
		-std=c++11 -g \
		@JSONCPP_CFLAGS@ \
		@ILMCONTROL_CFLAGS@ \
		@ILMCOMMON_CFLAGS@ \
		@ILMINPUT_CFLAGS@ 

# ビルドする実行ファイル(exsample)のリンカオプションの指定を行う。
ilmmanager_LDFLAGS = \
		-lrt -lpthread \
		@JSONCPP_LIBS@ \
		@ILMCONTROL_LIBS@ \
		@ILMCOMMON_LIBS@ \
		@ILMINPUT_LIBS@ 


# configureオプションによって変更する設定の記述。
if ENABLE_USERDEBUG
ilmmanager_CFLAGS   += -D_USER_DEBUG_
ilmmanager_CXXFLAGS += -D_USER_DEBUG_
endif
