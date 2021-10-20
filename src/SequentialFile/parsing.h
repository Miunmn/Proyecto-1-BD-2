#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>

#pragma once
// struct Registro {
    
//     char codigo [6];
//     char nombre [20];
//     char carrera [20];
//     int ciclo;
//     int next = 0;

//     void setData() {
//         std::cout << "Codigo:\n";
//         std::cin >> codigo;
//         std::cout << "Nombre:\n";
//         std::cin >> nombre;
//         std::cout << "Carrera:\n";
//         std::cin >> carrera;
//         std::cout << "Ciclo:\n";
//         std::cin >> ciclo;
//         std::cout << "Next:\n";
//         std::cin >> next;
//     }

//     void show() {
//         std::cout << '\n';
//         std::cout << codigo << '\n';
//         std::cout << nombre << '\n';
//         std::cout << carrera << '\n';
//         std::cout << ciclo << '\n';
//         std::cout << next << '\n';
//         std::cout << '\n';
//     }

// };
struct Population
{
    int rank;
    char country[40];
    float density;
    float densityMi;
    float pop2021;
    int area;
    int next;

    Population(){};

    Population(int rank_, std::string country_, float density_, float densityMi_, float pop2021_, int area_)
    {
        this->rank = rank_;
        std::strcpy(this->country, country_.c_str());
        this->density = density_;
        this->pop2021 = pop2021_;
        this->densityMi = densityMi_;
        this->area = area_;
        this->next = -1; 
    }
    void show()
    {
        std::cout << rank << " ";
        std::cout << country << " ";
        std::cout << density << " ";
        std::cout << densityMi << " ";
        std::cout << pop2021<< " ";
        std::cout << area << " ";
        std::cout << next << "\n";
    }
};
struct University
{
    int rank;
    int rank_reg;
    char university_name[60];
    int score;
    float alumni;
    University(int rank_, int rank_reg_, std::string university_name_, int score_, float alumni_)
    {
        this->rank = rank_;
        this->rank_reg = rank_reg_;
        std::strcpy(this->university_name, university_name_.c_str());
        this->score = score_;
        this->alumni = alumni_;
    }
    void show()
    {
        std::cout << rank << " ";
        std::cout << rank_reg << " ";
        std::cout << university_name << " ";
        std::cout << score << " ";
        std::cout << alumni << "\n";
    }
};

class CSVRow
{
public:
    std::string operator[](std::size_t index) const
    {
        return std::string(&m_line[m_data[index] + 1], m_data[index + 1] - (m_data[index] + 1));
    }
    std::size_t size() const
    {
        return m_data.size() - 1;
    }
    void readNextRow(std::istream &str)
    {
        std::getline(str, m_line);

        m_data.clear();
        m_data.emplace_back(-1);
        std::string::size_type pos = 0;
        while ((pos = m_line.find(',', pos)) != std::string::npos)
        {
            m_data.emplace_back(pos);
            ++pos;
        }
        pos = m_line.size();
        m_data.emplace_back(pos);
    }

private:
    std::string m_line;
    std::vector<int> m_data;
};
std::istream &operator>>(std::istream &str, CSVRow &data)
{
    data.readNextRow(str);
    return str;
}
std::vector<Population> populations_vector(int seq = 0){
    
    std::ifstream data_file;
    std::string population_csv = "population.csv";
    std::string rank_university_csv = "rank-univ.csv";

    std::ifstream population_csvfile(population_csv);

    CSVRow row;
    int i = 0;

    std::vector<Population> populations;

    if(seq == 0){
        while (population_csvfile >> row)
        {
            if (i == 0)
            {
                i++;
                continue;
            }
            int rank = std::stoi(row[0]);
            std::string country = row[1];
            float density = std::stof(row[2]);
            float densityMi = std::stof(row[3]);
            float pop2021 = std::stof(row[4]);
            int area = std::stoi(row[5]);
            Population population(rank, country, density, densityMi, pop2021, area);
            populations.push_back(population);
        }
    }else{
        while (population_csvfile >> row)
        {
            if (i == 0)
            {
                i++;
                continue;
            }
            int rank = std::stoi(row[0]);
            std::string country = row[1];
            float density = std::stof(row[2]);
            float densityMi = std::stof(row[3]);
            float pop2021 = std::stof(row[4]);
            int area = std::stoi(row[5]);
            Population population(rank, country, density, densityMi, pop2021, area);
            populations.push_back(population);
        }
    }
    return populations;
}


std::vector<University> universities_vector(){

    std::ifstream data_file;
    std::string rank_university_csv = "rank-univ.csv";

    CSVRow row;
    int i = 0;

    std::vector<University> universities;


    std::ifstream ranking_csvfile(rank_university_csv);
    i = 0;
    while (ranking_csvfile >> row)
    {
        if (i == 0)
        {
            i++;
            continue;
        }
        int rank = std::stoi(row[0]);
        int rank_reg = std::stoi(row[1]);
        std::string university_name = row[2];
        int score = std::stoi(row[3]);
        int alumni = std::stoi(row[4]);

        University university(rank, rank_reg, university_name, score, alumni);
        universities.push_back(university);
    }

    return universities;
    // std::vector<Population> populations;  --> Done
}



// int main()
// {
//     std::ifstream data_file;
//     std::string population_csv = "population.csv";
//     std::string rank_university_csv = "rank-univ.csv";

//     std::ifstream population_csvfile(population_csv);

//     CSVRow row;
//     int i = 0;

//     std::vector<Population> populations;
//     std::vector<University> universities;

//     while (population_csvfile >> row)
//     {
//         if (i == 0)
//         {
//             i++;
//             continue;
//         }
//         int rank = std::stoi(row[0]);
//         std::string country = row[1];
//         float density = std::stof(row[2]);
//         float densityMi = std::stof(row[3]);
//         float pop2021 = std::stof(row[4]);
//         int area = std::stoi(row[5]);
//         Population population(rank, country, density, densityMi, pop2021, area);
//         populations.push_back(population);
//     }

//     std::ifstream ranking_csvfile(rank_university_csv);
//     i = 0;
//     while (ranking_csvfile >> row)
//     {
//         if (i == 0)
//         {
//             i++;
//             continue;
//         }
//         int rank = std::stoi(row[0]);
//         int rank_reg = std::stoi(row[1]);
//         std::string university_name = row[2];
//         int score = std::stoi(row[3]);
//         int alumni = std::stoi(row[4]);

//         University university(rank, rank_reg, university_name, score, alumni);
//         universities.push_back(university);
//     }


//     // std::vector<Population> populations;  --> Done
//     // std::vector<University> universities; --> Done
//     return 0;
// }
