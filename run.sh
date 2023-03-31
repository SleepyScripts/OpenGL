gcc -o out main.c src/window.c src/shader.c src/camera.c -Iheaders -lGL -lGLU -lGLEW -lglfw -lm -lcglm -lassimp
./out
