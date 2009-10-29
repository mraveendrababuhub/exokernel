typedef union {long itype; tree ttype; char *strtype; enum tree_code code; } YYSTYPE;
#define	IDENTIFIER	258
#define	TYPENAME	259
#define	SCSPEC	260
#define	TYPESPEC	261
#define	TYPE_QUAL	262
#define	CONSTANT	263
#define	STRING	264
#define	ELLIPSIS	265
#define	SIZEOF	266
#define	ENUM	267
#define	IF	268
#define	ELSE	269
#define	WHILE	270
#define	DO	271
#define	FOR	272
#define	SWITCH	273
#define	CASE	274
#define	DEFAULT	275
#define	BREAK	276
#define	CONTINUE	277
#define	RETURN	278
#define	GOTO	279
#define	ASM_KEYWORD	280
#define	GCC_ASM_KEYWORD	281
#define	TYPEOF	282
#define	ALIGNOF	283
#define	SIGOF	284
#define	ATTRIBUTE	285
#define	EXTENSION	286
#define	LABEL	287
#define	AGGR	288
#define	VISSPEC	289
#define	DELETE	290
#define	NEW	291
#define	OVERLOAD	292
#define	THIS	293
#define	OPERATOR	294
#define	CXX_TRUE	295
#define	CXX_FALSE	296
#define	NAMESPACE	297
#define	TYPENAME_KEYWORD	298
#define	USING	299
#define	LEFT_RIGHT	300
#define	TEMPLATE	301
#define	TYPEID	302
#define	DYNAMIC_CAST	303
#define	STATIC_CAST	304
#define	REINTERPRET_CAST	305
#define	CONST_CAST	306
#define	SCOPE	307
#define	EMPTY	308
#define	PTYPENAME	309
#define	NSNAME	310
#define	THROW	311
#define	ASSIGN	312
#define	OROR	313
#define	ANDAND	314
#define	MIN_MAX	315
#define	EQCOMPARE	316
#define	ARITHCOMPARE	317
#define	LSHIFT	318
#define	RSHIFT	319
#define	POINTSAT_STAR	320
#define	DOT_STAR	321
#define	UNARY	322
#define	PLUSPLUS	323
#define	MINUSMINUS	324
#define	HYPERUNARY	325
#define	PAREN_STAR_PAREN	326
#define	POINTSAT	327
#define	TRY	328
#define	CATCH	329
#define	TYPENAME_ELLIPSIS	330
#define	PRE_PARSED_FUNCTION_DECL	331
#define	EXTERN_LANG_STRING	332
#define	ALL	333
#define	PRE_PARSED_CLASS_DECL	334
#define	TYPENAME_DEFN	335
#define	IDENTIFIER_DEFN	336
#define	PTYPENAME_DEFN	337
#define	END_OF_LINE	338
#define	END_OF_SAVED_INPUT	339


extern YYSTYPE yylval;
#define YYEMPTY		-2