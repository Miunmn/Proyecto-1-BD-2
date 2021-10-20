#include "../SequentialFile/parsing.h";
#include "test.h";

using namespace std;

int main(){

    std::vector <Population> populations = populations_vector();

    // fb settear a 7
    int D = 5;
    string index_filename = "index_popu.dat";
    string data_filename = "data_popu.dat";

    extendible_hash<Population, bucket_population> hash (D, index_filename, data_filename);
    // for (Population p : populations) {
    //     hash.insert_register(p);
    // }
    auto aaa = hash.select_all();
    for (auto a: aaa){
        a.show();
    }

return 0;
}
