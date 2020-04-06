#sed -n -e '/<title>/p' -e '/<text>/,/<\/text>/p' from.xml
sed -n 'w 111.xml' from.xml
sed -i ':begin; /AA::AA()/,/{/ { /{/! { $! {N; b begin};}; s/AA::AA().*{/&\r\n\tnew b;/;};' 111.xml
#sed -i -f func.sed 111.xml
