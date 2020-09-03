del /f "Mafia\Network\CPP_LIB\SubserverNetworker.so"

g++^
 -I"C:\Program Files\Java\jdk-9.0.4\include"^
 -I"..\..\..\..\..\StructParser"^
 -I"..\..\..\..\Server"^
 -I"Mafia\Network\CPP_SOURCES\RoomServerSources"^
 -fPIC -D DONT_USE_QT^
 "Mafia\Network\CPP_SOURCES\RoomServerSources\IncludeList.cpp"^
 "..\..\..\..\Server\System\functions.cpp"^
 "..\..\..\..\Server\System\simplestring.cpp"^
 "..\..\..\..\Server\System\keygen.cpp"^
 "..\..\..\..\Server\System\tuple.cpp"^
 "..\..\..\..\Server\Exceptions\*.cpp"^
 "..\..\..\..\Server\Database\user.cpp"^
 "..\..\..\..\Server\Database\chatmessage.cpp"^
 "..\..\..\..\Server\Database\chatsettings.cpp"^
 "..\..\..\..\Server\Gameplay\game_s.cpp"^
 "..\..\..\..\Server\userstatistics.cpp"^
 "..\..\..\..\Server\clientinfo.cpp"^
 "..\..\..\..\Server\System\serializer.cpp"^
 "..\..\..\..\Server\Network\crypto.cpp"^
 "..\..\..\..\Server\Network\mainservernetworker.cpp"^
 "Mafia\Network\CPP_sources\RoomServerSources\includemessagesender.cpp"^
 "Mafia\Network\CPP_sources\RoomServerSources\Summator.cpp"^
 "Mafia\Network\CPP_SOURCES\RoomServerSources\jnimethodrunner.cpp"^
 "Mafia\Network\CPP_sources\RoomServerSources\SubserverNetworker.cpp"^
 -shared -o^
 "Mafia\Network\CPP_LIB\SubserverNetworker.so"
