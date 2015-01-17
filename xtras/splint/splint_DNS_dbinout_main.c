bash-3.00$ splint $(mysql_config --cflags) $(mysql_config --libs) DNS_client_main.c -I../include -warnposix -maintype
Splint 3.1.1 --- 15 Jun 2004

DNS_dbinout_main.c:47:9: Parse Error. (For help on parse errors, see splint
               -help parseerrors.)
*** Cannot continue.
