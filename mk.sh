g++ -Ofast -Os -m64 -std=c++2a -Wall -Wextra -Wpedantic -c src/*.cpp
g++ -Ofast -Os -m64 -std=c++2a -Wall -Wextra -Wpedantic -o scampana *.o imgui/*.o -lGL -lglfw -lm -lpthread
rm *.o

x86_64-w64-mingw32-g++ -Ofast -Os -m64 -std=c++2a -Wall -Wextra -Wpedantic -c src/*.cpp -I vendor/include
x86_64-w64-mingw32-g++ -Ofast -Os -m64 -std=c++2a -Wall -Wextra -Wpedantic -o scampana.exe *.o imgui/win/*.o -I vendor/include -L vendor/lib -lopengl32 -lglfw3 -lkernel32 -luser32 -lgdi32 -lm
rm *.o