#include "simulation.hpp"

void showADN(std::vector<std::vector<int>> ADN){
    for (uint32_t i=0; i < ADN.size(); i++){
        std::cout << "gene " << i  << " : ";
        for (uint32_t j=0; j < ADN[i].size()-1; j++){
            std::cout << ADN[i][j] << "-";
        }
        std::cout << ADN[i][ADN[i].size()-1] << std::endl;
    }
}

sf::Color wheelColor(int n){
    int r, g, b;
    if (n < 20){
        r = 255;
        g = (n%20)/20 * 255;
        b = 0;
    }
    else if (20 <= n && n < 40){
        r = 255 - (n%20)/20 * 255;
        g = 255;
        b = 0;
    } else if (40 <= n && n < 60){
        r = 0;
        g = 255;
        b = (n%20)/20 * 255;
    } else if (60 <= n && n < 80){
        r = 0;
        g = 255 - (n%20)/20 * 255;
        b = 255;
    } else if (80 <= n && n < 100){
        r = (n%20)/20 * 255;
        g = 0;
        b = 255;
    } else if (100 <= n && n < 120){
        r = 255;
        g = 0;
        b = 255 - (n%20)/20 * 255;
    }
    return sf::Color(r, g, b);
}

sf::Color randomColor(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 120);
    return wheelColor(dis(gen));
}

void setADN(Tree& tree, short n){
    if (n==0){
        tree.ADN = {
            {1, 30, 30, 30},
            {2, 30, 30, 30},
            {1, 3, 30, 4},
            {30, 3, 30, 30},
            {30, 30, 30, 4},
            {5, 5, 5, 5},
            {6, 6, 6, 6},
            {7, 7, 7, 7},
            {8, 8, 8, 8},
            {9, 9, 9, 9},
            {10,10,10,10},
            {11,11,11,11},
            {12,12,12,12},
            {13,13,13,13},
            {14,14,14,14},
            {15,15,15,15},
            {16,16,16,16},
            {17,17,17,17},
            {18,18,18,18},
            {19,19,19,19}
        };
    } else if (n==1){
        tree.ADN = {
            {1, 30, 30, 30},
            {30, 30, 30, 4},
            {1, 3, 30, 6},
            {30, 3, 30, 30},
            {2, 30, 30, 30},
            {30, 30, 30, 5},
            {30, 30, 18, 5},
            {7, 7, 7, 7},
            {8, 8, 8, 8},
            {9, 9, 9, 9},
            {10,10,10,10},
            {11,11,11,11},
            {12,12,12,12},
            {13,13,13,13},
            {14,14,14,14},
            {15,15,15,15},
            {16,16,16,16},
            {17,17,17,17},
            {19,30,30,30},
            {30,30,30,30}
        };
    }
};

Simulation::Simulation(){
    world.resize(conf::lines);
    for (int i = 0; i < conf::lines; ++i) {
        world[i].resize(conf::columns);
        for (int j = 0; j < conf::columns; ++j) {
            world[i][j] = Cell(sf::Vector2i{j, i});
        }
    }

    light.resize(conf::lines);
    for (int i = 0; i < conf::lines; ++i) {
        light[i].resize(conf::columns);
    }
}

void Simulation::throwSeeds(int n){
    n = n % conf::columns;

    //random numbers generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, conf::columns - 1);
    
    for (int index = 0; index < n; index++) {
        int j = dis(gen);
        int i = conf::lines - 1;
        
        // Ensure the cell is empty before placing a seed
        if (world[i][j].type == 0) {
            world[i][j].energy = conf::startEnergy;

            newTree(&world[i][j], randomColor(), randomADN());
            //showADN(trees.back()->ADN);

            world[i][j].type = 3; // seed
        }
        else index--;
    }

    // setADN(trees[0], 0);
    // setADN(trees[1], 1);
}

// MARK: Energy
std::vector<std::vector< int >> Simulation::updateSunLight(std::vector<std::vector< int >> & lightMatrix){
    for (int j = 0; j < conf::columns; ++j) {
        float recieved = 1.0f;
        for (int i = 0; i < conf::lines; ++i) {
            Cell* cell = &world[i][j];

            lightMatrix[i][j] = recieved * conf::light;

            if (cell->type == 0){
                if (recieved < 1.0f){
                    recieved += conf::lightIncrease;
                    recieved = std::min(recieved, 1.0f);
                }
            } else {
                if (recieved > 0.0f){
                    recieved *= conf::lightThroughLayer;
                }
            }
        }
    }

    return lightMatrix;
}

void Simulation::produceEnergy(Cell* cell, const std::vector<std::vector< int >> & lightMatrix){
    int light = lightMatrix[cell->position.y][cell->position.x];

    if (cell->type == 1){
        Tree& tree = trees[cell->treeId];
        if (tree.numberOfSprouts > 0) {
            tree.energy += light - tree.energy / tree.numberOfSprouts;
            cell->energy += tree.energy / tree.numberOfSprouts;
        }
        else {
            throw std::runtime_error("Tree " + std::to_string(cell->treeId) + " has no sprouts but a sprout cell exists.");
        }
    }
    else if (cell->type == 2){
        trees[cell->treeId].energy += light;
    }
}

bool Simulation::useEnergy(Cell* cell){
    Tree& tree = trees[cell->treeId];

    tree.energy -= conf::energy_cunsumption;

    if (tree.energy < 0){
        return false;
    }
    return true;
}

void Simulation::fall(Cell* cell){
    Cell* belowCell = &world[cell->position.y + 1][cell->position.x];
    if (belowCell->type != 0) return;

    birth(belowCell, cell, cell->type, cell->energy, cell->activeGene);

    trees[cell->treeId].cells = {belowCell};

    cell->type = 0; // dead
}

// MARK: birth / death

void Simulation::birth(Cell* newCell, Cell* cell, int inType, int energy, int inActiveGene) {
    newCell->treeId = cell->treeId;
    newCell->type = inType;
    newCell->energy = energy;
    newCell->activeGene = inActiveGene;
    
    trees[cell->treeId].cells.push_back(newCell);

    if (inType == 1) trees[cell->treeId].numberOfSprouts++;
}

void Simulation::dieTree(siv::ID treeId){
    Tree& tree = trees[treeId];

    for (Cell* cell : tree.cells) {
        cell->type = 0; // Set type to dead
    }
    // Remove the tree from the trees vector
    trees.erase(treeId);
}

void Simulation::spreadSeed(siv::ID treeId){
    for (uint32_t i = 0; i < trees[treeId].cells.size(); i++) {
        Cell* cell = trees[treeId].cells[i];
        if (cell->type == 1){
            cell->type = 3;
            newTree(cell, trees[treeId].color, trees[treeId].ADN);
        }
        else{
            cell->type = 0;
        }
    }
    // std::cout << "Tree " << treeId << " spread its seeds." << std::endl;
    // Remove the tree from the trees vector
    trees.erase(treeId);
    // std::cout << "Tree " << treeId << " erased." << std::endl;
}

// MARK: Genetic
void Simulation::newTree(Cell* seed, sf::Color color, const std::vector<std::vector<int>> inADN) {
    siv::ID id = trees.push_back(Tree(seed, color));
    Tree& tree = trees[id];
    seed->treeId = id;
    seed->activeGene = 0;
    tree.energy = seed->energy; // Transfer energy from the seed to the new tree
    tree.ADN = mutation(inADN);
    if (tree.ADN != inADN){
        tree.color = randomColor();
    }
}

std::vector<std::vector<int>> Simulation::randomADN() {
    std::vector<std::vector<int>> ADN(conf::genomeLength, std::vector<int>(4, 0));

    // Randomly initialize the ADN with values between 0 and conf::genomeLength
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> valueDis(0, conf::genomeLength * 2 - 1);

    for (int i = 0; i < conf::genomeLength; ++i) {
        for (int j = 0; j < 4; j++) {
            ADN[i][j] = valueDis(gen);
        }
    }
    
    return ADN;
}

std::vector<std::vector<int>> Simulation::mutation(std::vector<std::vector<int>> ADN) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> probaDis(1, 100);
    if ( probaDis(gen) <= conf::mutationRate ){

        std::uniform_int_distribution<int> geneDis(0, conf::genomeLength-1);
        std::uniform_int_distribution<int> genePartDis(0, 4);

        std::uniform_int_distribution<int> valueDis(0, conf::genomeLength * 2 - 1);
        ADN [ geneDis(gen) ] [ genePartDis(gen) ] = valueDis(gen);

    }

    return ADN;
}

//MARK: growth
void Simulation::grow(Cell* cell) {
    Tree& tree = trees[cell->treeId];
    
    std::vector< bool > growCells {false, false, false, false};
    int numberCells = 0;
    bool activeSprout = false;

    const std::vector< sf::Vector2i > directions = {
        {0, -1}, // up
        {1, 0},  // right
        {0, 1},  // down
        {-1, 0}  // left
    };

    for (int i = 0; i < directions.size(); i++) {
        sf::Vector2i newPosition = cell->position + directions[i];
        newPosition.x = (newPosition.x + conf::columns) % conf::columns; // Wrap around horizontally
        if (newPosition.y < 0 || newPosition.y >= conf::lines) continue; // Skip out of bounds positions
        
        if (tree.ADN[cell->activeGene][i] < conf::genomeLength){
            if (!activeSprout) activeSprout = true;

            if (world[newPosition.y][newPosition.x].type == 0){
                growCells[i] = true;
                numberCells++;
            }
        }
    }
    
    if (numberCells == 0) {
        if (activeSprout) return;

        cell->type = 2; // leaf
        tree.numberOfSprouts--;
        return;
    }
    if ( cell->energy >= numberCells * conf::newCellCost){
        cell->type = 2; // leaf
        tree.numberOfSprouts--;

        int newCellEnergy = (cell->energy - numberCells * conf::newCellCost) / numberCells;
        cell->energy = 0;

        for (int i = 0; i < directions.size(); i++) {
            if (growCells[i]){
                sf::Vector2i newPosition = cell->position + directions[i];
                newPosition.x = (newPosition.x + conf::columns) % conf::columns;
                
                int gene = tree.ADN[cell->activeGene][i];
                
                birth(&world[newPosition.y][newPosition.x], cell, 1, newCellEnergy, gene);
            }
        }
    }
}

// MARK: Step
void Simulation::step(){
    if ( !(stepCounter%conf::lifetime) ){
        generation++;
        std::cout << "--------------------- generation " << generation <<  " ---------------------" << std::endl;
    }
    
    if (!stepCounter)
    updateSunLight(light);

    for (uint32_t idx = 0; idx < trees.size(); idx++) {
        if( trees.data()[idx].cells.empty() ){
            std::cout << "Warning: Tree " << idx << " has no cells but still exists." << std::endl;
        }
        int i = trees.data()[idx].cells[0]->treeId;
        trees[i].age++;
        if ( trees[i].age > conf::lifetime ) {
            spreadSeed(i);
            idx--;
            continue;
        }
        int n = trees[i].cells.size();
        for (uint32_t j = 0; j < n; j++) {
            // std::cout << "Tree " << i << ", cell " << j << "/" << trees[i].cells.size() << "\n";
            // std::cout << trees[i].cells[j] << std::endl;

            Cell* exeCell = trees[i].cells[j];
            produceEnergy(exeCell, light);

            if (!useEnergy(exeCell)){
                dieTree(i);
                idx--;
                break;
            }
            
            switch (exeCell->type){
                case 1: // sprout
                    grow(exeCell);
                    break;
    
                case 2: // leaf
                    if (trees[i].numberOfSprouts == 0){
                        dieTree(i);
                        // used to end the for loop and avoid accessing the tree that has just been erased
                        j = n;
                        idx--;
                    }
                    break;
    
                case 3: // seed
                    if (exeCell->position.y < conf::lines-1) {
                        trees[i].age--;
                        fall(exeCell);
                    } else {
                        exeCell->type = 1; // sprout
                        trees[i].numberOfSprouts++;
                    }
                    break;
            }
        }
    }
    stepCounter++;
    updateSunLight(light);
    // std::cout << trees.size() << "/" << trees.capacity() << " trees remaining." << std::endl;
    // std::cout << "-------- end step --------" << std::endl;
}
