make
gnome-terminal -e "bash -c \"./bin/serveur 1234; exec bash\"" &
sleep 0.5
gnome-terminal -e "bash -c \"java -cp ./bin interfaceCJavaProlog 1244; exec bash\"" &
sleep 0.5
gnome-terminal -e "bash -c \"./bin/client 127.0.0.1 1234; exec bash\"" &
sleep 0.5
gnome-terminal -e "bash -c \"./bin/client 127.0.0.1 1234 Bob; exec bash\"" &

