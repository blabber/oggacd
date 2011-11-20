PROG=		oggacd

CSTD?=		c89
WARNS?=		6
WFORMAT?=	1
NO_MAN=		yes
NO_WERROR=	yes

CFLAGS+=	-I/usr/local/include
LDFLAGS+=	-L/usr/local/lib 
LDADD+=		-lvorbisfile

CLEANFILES=	*.[Bb][Aa][Kk] *.core
CTAGS=		ctags

.include <bsd.prog.mk>
