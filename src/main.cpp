#include <SFML/Graphics.hpp>
// #include "parsing.h"
#include "./extendible_hash/test.h"
#include "./SequentialFile/p1.h"


class MyButton : public sf::Drawable
{
public:
    sf::RectangleShape shape;
    sf::Text innerText;
    sf::Font font;

    MyButton(int posX, int posY, std::string text, int charSize = 40, int sizeX = 300, int sizeY = 50)
    {
        shape.setSize(sf::Vector2f(sizeX, sizeY));
        shape.setFillColor(sf::Color::Black);
        shape.setPosition(posX, posY);

        font.loadFromFile("arial.ttf");
        innerText.setString(text);
        innerText.setFont(font);
        innerText.setPosition(posX, posY);
        innerText.setCharacterSize(charSize);
    }

    void setString_(std::string text_)
    {
        innerText.setString(text_);
    }

    bool click(const sf::Event &event)
    {
        return shape.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y);
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(shape);
        target.draw(innerText);
    }
};

MyButton populationbutton(200, 150, "Population");
MyButton universitiesbutton(600, 150, "Universities");

MyButton extendible_hashbutton(200, 400, "Extendible Hash");
MyButton sequentia_filebutton(600, 400, "Sequential File");

MyButton query1_button(200, 600, "Insertar");
MyButton query2_button(400, 600, "Eliminar");
MyButton query3_button(600, 600, "Busqueda");
MyButton query4_button(800, 600, "Busqueda rango");
MyButton query5_button(1000, 600, "Mostrar Todos");

// sf::Text input1;
MyButton input1(740, 50, "<ingrese>", 20, 100, 40);

MyButton input2(300, 50, "<ingrese>");

void showMainMenu(sf::RenderWindow &window)
{
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text baseTitleText;
    baseTitleText.setString(L"Bienvenido al DBMS!");
    baseTitleText.setFont(font);
    baseTitleText.setCharacterSize(40);
    baseTitleText.setStyle(sf::Text::Bold);
    baseTitleText.setFillColor(sf::Color::Black);
    baseTitleText.setPosition(200.0f, 50.0f);

    sf::Text subTitleText;
    subTitleText.setString(L"¿Sobre cuál dataset desea consultar?");
    subTitleText.setFont(font);

    subTitleText.setCharacterSize(30);
    subTitleText.setStyle(sf::Text::Bold);
    subTitleText.setFillColor(sf::Color{0, 107, 96});
    subTitleText.setPosition(200.0f, 100.0f);

    window.draw(baseTitleText);
    window.draw(subTitleText);

    populationbutton.draw(window, sf::RenderStates());
    universitiesbutton.draw(window, sf::RenderStates());
}

void writeCell(sf::RenderWindow &window, int &width, int &prevCellWidth, int &row_iter, int &col_iter, std::string &stringText)
{
    //debugear porque solo la ultima columna sale ancha
    sf::Vector2f cellSize(width, 50); //(width, height)
    sf::Vector2f cellPos(prevCellWidth, 50);
    prevCellWidth = prevCellWidth + 20;
    // sf::RectangleShape grid[columns][rows];
    // grid[row_iter][col_iter].setSize(cellSize);
    // grid[row_iter][col_iter].setOutlineColor(sf::Color::Blue);
    // grid[row_iter][col_iter].setOutlineThickness(3.0f);

    float y_value = cellPos.y * row_iter;
    float x_value = cellPos.x * col_iter;

    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text text(stringText, font);
    text.setCharacterSize(20);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color{0, 107, 96});
    text.setPosition(x_value + cellPos.x, y_value + cellPos.y);

    // grid[row_iter][col_iter].setPosition(x_value + cellPos.x, y_value + cellPos.y);
    //  window.draw(grid[row_iter][col_iter]);
    window.draw(text);
}

void showAllPopulation(sf::RenderWindow &window, std::vector<Population> &dataset, int columns)
{
    // GRID

    for (int row_iter = 1; row_iter < dataset.size() + 1; row_iter++)
    {
        int prevCellWidth = 50;
        for (int col_iter = 1; col_iter < columns + 1; col_iter++)
        {
            std::string stringText = "";
            int extraWidth = 0;
            int width = 0;
            int littleMargin = 0;
            // float extraWidthToFitText = 0.0f;

            // 0: int rank, 1: country[40], 2: float density, 3: float densityMi, 4: float pop2021, 5: int area
            switch (col_iter)
            {
            case 1:
                stringText = std::to_string(dataset[row_iter - 1].rank);
                width = 40;
                break;
            case 2:
                width = 100;
                stringText = dataset[row_iter - 1].country;
                extraWidth = 0;
                break;
            case 3:
                // littleMargin = 15;
                width = 250;
                stringText = std::to_string(dataset[row_iter - 1].density);
                break;
            case 4:
                stringText = std::to_string(dataset[row_iter - 1].densityMi);
                break;
            case 5:
                stringText = std::to_string(dataset[row_iter - 1].pop2021);
                break;
            default:
                stringText = std::to_string(dataset[row_iter - 1].area);
                break;
            }
            //f::RenderWindow &window, int width, int prevCellWidth, int row_iter, int col_iter, std::string stringText
            writeCell(window, width, prevCellWidth, row_iter, col_iter, stringText);
        }
    }
}

void showAll2(sf::RenderWindow &window, std::vector<University> &dataset, int columns)
{
    for (int row_iter = 1; row_iter < dataset.size() + 1; row_iter++)
    {
        int prevCellWidth = 50;
        for (int col_iter = 1; col_iter < columns + 1; col_iter++)
        {
            std::string stringText = "";
            int extraWidth = 0;
            int width = 0;
            int littleMargin = 0;
            switch (col_iter)
            {
            case 1:
                stringText = std::to_string(dataset[row_iter - 1].rank);
                width = 20;
                break;
            case 2:
                stringText = std::to_string(dataset[row_iter - 1].rank_reg);
                width = 20;
                break;
            case 3:
                width = 20;
                stringText = dataset[row_iter - 1].university_name;
                extraWidth = 0;
                break;
            case 4:
                // littleMargin = 15;
                width = 100;
                stringText = std::to_string(dataset[row_iter - 1].score);
                break;
            default:
                stringText = std::to_string(dataset[row_iter - 1].alumni);
                break;
            }
            //f::RenderWindow &window, int width, int prevCellWidth, int row_iter, int col_iter, std::string stringText
            writeCell(window, width, prevCellWidth, row_iter, col_iter, stringText);
        }
    }
}

void showIndexOptions(sf::RenderWindow &window)
{
    sf::Text subTitleText;
    sf::Font font;
    font.loadFromFile("arial.ttf");

    subTitleText.setString(L"¿Qué índice desea usar?");
    subTitleText.setFont(font);

    subTitleText.setCharacterSize(30);
    subTitleText.setStyle(sf::Text::Bold);
    subTitleText.setFillColor(sf::Color{0, 107, 96});
    subTitleText.setPosition(200.0f, 300.0f);
    window.draw(subTitleText);

    extendible_hashbutton.draw(window, sf::RenderStates());
    sequentia_filebutton.draw(window, sf::RenderStates());
}


void returnMessage(sf::RenderWindow &window, std::string text){
    sf::Text subTitleText;
    sf::Font font;
    font.loadFromFile("arial.ttf");

    subTitleText.setString(text);
    subTitleText.setFont(font);

    subTitleText.setCharacterSize(40);
    subTitleText.setStyle(sf::Text::Bold);
    subTitleText.setFillColor(sf::Color{0, 107, 96});
    subTitleText.setPosition(950.0f, 50.0f);
    window.draw(subTitleText);
}

void showqueryInterface(sf::RenderWindow &window, int queryOpt, std::string tabla)
{
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text queryText;
    float y_value = 50.0f;
    //                        window.draw(input1);

    switch (queryOpt)
    {
    case 1:
        // INSERT INTO <tabla> INTO (<keyColumn>) VALUES (<key>);
        queryText.setString("INSERT INTO " + tabla + " ( key ) VALUES (            )");
        queryText.setFont(font);
        queryText.setCharacterSize(25);
        queryText.setStyle(sf::Text::Bold);
        queryText.setFillColor(sf::Color{0, 107, 96});
        queryText.setPosition(200.0f, y_value);
        window.draw(input1);
        break;
    case 2:
        // DELETE FROM <tabla> where key=<key>
        queryText.setString("SELECT * FROM " + tabla + " WHERE KEY=");
        queryText.setFont(font);
        queryText.setCharacterSize(25);
        queryText.setStyle(sf::Text::Bold);
        queryText.setFillColor(sf::Color{0, 107, 96});
        queryText.setPosition(200.0f, y_value);
        break;
    case 3:
        // SELECT * FROM <tabla> where key=<key>
        queryText.setString("SELECT * FROM " + tabla + " WHERE KEY=");
        queryText.setFont(font);
        queryText.setCharacterSize(25);
        queryText.setStyle(sf::Text::Bold);
        queryText.setFillColor(sf::Color{0, 107, 96});
        queryText.setPosition(200.0f, y_value);
    case 4:
        // SELECT * FROM <tabla> WHERE <min> < key AND  key < <max>;
        queryText.setString("SELECT * FROM " + tabla + " WHERE         < key AND  key <       ;");
        queryText.setFont(font);
        queryText.setCharacterSize(25);
        queryText.setStyle(sf::Text::Bold);
        queryText.setFillColor(sf::Color{0, 107, 96});
        queryText.setPosition(200.0f, y_value);
        break;
    case 5:
        // SELECT * FROM <tabla>
        queryText.setString("SELECT * FROM " + tabla);
        queryText.setFont(font);
        queryText.setCharacterSize(25);
        queryText.setStyle(sf::Text::Bold);
        queryText.setFillColor(sf::Color{0, 107, 96});
        queryText.setPosition(200.0f, y_value);
        break;
    default:
        break;
    }

    window.draw(queryText);
}

void showCrudButtons(sf::RenderWindow &window, int indexEscogido)
{
    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text consulta;
    consulta.setString("Escoja operacion");
    consulta.setFont(font);
    consulta.setCharacterSize(25);
    consulta.setStyle(sf::Text::Bold);
    consulta.setFillColor(sf::Color{0, 107, 96});
    consulta.setPosition(200.0f, 500.0f);

    window.draw(consulta);
    window.draw(query1_button);
    window.draw(query2_button);
    window.draw(query3_button);
    if (indexEscogido == 1)
    {
        window.draw(query4_button);
    }
    window.draw(query5_button);
}

int main()
{
    int D = 5;
    string index_filename_pop = "extendible_hash/index_popu.dat";
    string data_filename_pop = "extendible_hash/data_popu.dat";

    // string index_filename_uni = "extendible_hash/index_uni.dat";
    // string data_filename_uni = "extendible_hash/data_uni.dat";

    extendible_hash<Population, bucket_population> hashPop(D, index_filename_pop, data_filename_pop);


    // extendible_hash<University, bucket_population> hashUni (D, index_filename, data_filename);

    std::vector<Population> populations;

    sf::RenderWindow window(sf::VideoMode(1400, 1200), "Proyecto 1 BD2");
    window.clear(sf::Color::White);
    showMainMenu(window);
    sf::RectangleShape header;
    header.setSize(sf::Vector2f(2000, 50));
    header.setFillColor(sf::Color{229, 160, 0});
    header.setPosition(0, 0);
    window.draw(header);

    // std::vector<Population> populations;
    std::vector<University> universities;

    int datasetOpt = 0;
    int idxOpt = 0;
    int queryOpt = 0;
    std::string inputText = "";

    int rows = 0;
    int columns = 0;

    bool datasetEscogido = false;
    bool indexEscogido = false;
    bool queryOptEscogido = false;
    bool finalInputEntered = false;

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (populationbutton.click(event))
                {
                    datasetOpt = 1;
                }

                if (universitiesbutton.click(event))
                {
                    datasetOpt = 2;
                }

                if (sequentia_filebutton.click(event))
                {
                    idxOpt = 1;
                    indexEscogido = true;
                }

                if (extendible_hashbutton.click(event))
                {
                    idxOpt = 2;
                    indexEscogido = true;
                }

                if (query1_button.click(event))
                {
                    queryOpt = 1;
                    queryOptEscogido = true;
                }

                if (query2_button.click(event))
                {
                    queryOpt = 2;
                    queryOptEscogido = true;
                }
                if (query3_button.click(event))
                {
                    queryOpt = 3;
                    queryOptEscogido = true;
                }
                if (query4_button.click(event))
                {
                    queryOpt = 4;
                    queryOptEscogido = true;
                }
                if (query5_button.click(event))
                {
                    queryOpt = 5;
                    queryOptEscogido = true;
                    finalInputEntered = true;
                }
            }
            if (event.type == sf::Event::TextEntered)
            {
                if (queryOptEscogido && event.text.unicode < 128)
                {
                    if (event.text.unicode == '\b' && inputText.size() > 0)
                    {
                        inputText.pop_back();
                        input1.setString_(inputText);
                        window.draw(input1);
                    }
                    else if (event.text.unicode == '\r')
                    {
                        switch (queryOpt)
                        {
                        case 1:
                            finalInputEntered = true;
                            break;
                        default:
                            break;
                        }
                    }
                    else if (event.text.unicode != '\b')
                    {
                        inputText += event.text.unicode;
                        input1.setString_(inputText);
                        window.draw(input1);
                    }
                }
            }
            // datasetOpt = populationbutton.click(event)?1: universitiesbutton.click(event)?2:0;
            // idxOpt = sequentia_filebutton.click(event)?1: extendible_hashbutton.click(event)?2:0;
        }

        if (!datasetEscogido)
        {
            switch (datasetOpt)
            {
            case 1:

                showIndexOptions(window);
                if (idxOpt != 0)
                {
                    showCrudButtons(window, idxOpt);
                    if (queryOpt)
                    {
                        window.clear(sf::Color::White);
                        showqueryInterface(window, queryOpt, "populations");
                        if (finalInputEntered)
                        {
                            switch (queryOpt)
                            {
                                case 1:
                                    if(idxOpt == 2){
                                        // Population newPop();
                                        // hashPop.insert_register(newPo);
                                        showAllPopulation(window, populations, columns);
                                        returnMessage(window, "Registro Ingresado!");
                                    }
                                case 5:
                                    if (idxOpt == 2){
                                        populations = hashPop.select_all();
                                        columns = 6;
                                        showAllPopulation(window, populations, columns);
                                    }
                                    else if (idxOpt == 1){
                                        populations = useSeqFile(5);
                                        columns = 6;
                                        showAllPopulation(window, populations, columns);
                                    }
                                    break;

                            default:
                                break;
                            }
                        }
                    }
                    // showqueryInterface(window, idxOpt, "populations");
                }
                break;
            case 2:
                // universities = universities_vector();
                columns = 5;
                rows = universities.size();
                showIndexOptions(window);
                if (idxOpt != 0)
                {
                    showCrudButtons(window, idxOpt);
                    if (queryOptEscogido)
                    {
                        window.clear(sf::Color::White);
                        showqueryInterface(window, queryOpt, "universities");
                    }
                    // showqueryInterface(window, idxOpt, "universities");
                }
                // showAll1(window, universities, columns);
                break;
            default:
                break;
            }
        }
        window.display();
    }
    return 0;
}
