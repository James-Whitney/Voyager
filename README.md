Voyager


#### Sean's Shity SFML (s)Fix
In build/src/voyager-{server/client}/CMakeFiles/voyager-client.dir/link.txt change the two instances of -lsfml to `../../ext/sfml/lib/libsfml-audio.a ../../ext/sfml/lib/libsfml-graphics.a  ../../ext/sfml/lib/libsfml-network.a ../../ext/sfml/lib/libsfml-system.a ../../ext/sfml/lib/libsfml-window.a`
