LIBS=-lsfml-graphics -lsfml-window -lsfml-system

build:
	@echo "** Building sfml"
	g++ -c "main.cpp" -o main.o
	g++ main.o -o sfml-project-executable $(LIBS)

run: sfml-project-executable
	@echo "** Running! "
	./sfml-project-executable

clean:
	@echo "** Removing object files and executable..."
	rm -f main.o sfml-project-executable


