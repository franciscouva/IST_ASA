#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Função para calcular o valor máximo ao vender peças cortadas
int calculateMaxSellValue(int sideMax, int sideMin, const vector<vector<int>>& piecesValues);

// Função para calcular o valor máximo ao cortar peças
int calculateMaxCutValue(int row, int col, const vector<vector<int>>& maxValues);

int main() {
    int firstSize, secondSize, n;

    // Leitura das dimensões da grade e número de peças
    scanf("%d %d", &firstSize, &secondSize);
    scanf("%d", &n);

    // Determinação do lado mínimo e máximo
    int sideMin = min(firstSize, secondSize);
    int sideMax = max(firstSize, secondSize);

    // Inicialização da matriz de valores das peças
    vector<vector<int>> piecesValues(sideMin + 1, vector<int>(sideMax + 1, 0));

    // Leitura e armazenamento das dimensões e valores das peças
    for (int i = 0; i < n; i++) {
        int dimX, dimY, value;

        scanf("%d %d %d", &dimX, &dimY, &value);

        // Armazenamento do valor máximo para cada combinação de dimensões
        if (dimX <= sideMin && dimY <= sideMax)
            piecesValues[dimX][dimY] = max(value, piecesValues[dimX][dimY]);
        if (dimX <= sideMax && dimY <= sideMin)
            piecesValues[dimY][dimX] = max(value, piecesValues[dimY][dimX]);
    }

    // Cálculo e impressão do valor máximo ao vender peças cortadas
    cout << calculateMaxSellValue(sideMax, sideMin, piecesValues) << '\n';

    return 0;
}

// Função para calcular o valor máximo ao vender peças cortadas
int calculateMaxSellValue(int sideMax, int sideMin, const vector<vector<int>>& piecesValues) {
    // Inicialização da matriz para armazenar os valores máximos
    vector<vector<int>> maxValues(sideMin + 1, vector<int>(sideMax + 1, 0));

    // Preenchimento da matriz com os valores máximos possíveis
    for (int row = 1; row <= sideMin; row++) {
        for (int col = 1; col <= sideMax; col++) {
            // O valor máximo é o máximo entre o valor da peça original e o valor máximo ao cortar
            maxValues[row][col] = max(piecesValues[row][col], calculateMaxCutValue(row, col, maxValues));

            // Garante simetria, já que as peças podem ser giradas
            if (col <= sideMin)
                maxValues[col][row] = maxValues[row][col];
        }
    }

    // Retorna o valor máximo ao vender peças cortadas
    return maxValues[sideMin][sideMax];
}

// Função para calcular o valor máximo ao cortar peças
int calculateMaxCutValue(int row, int col, const vector<vector<int>>& maxValues) {
    // Inicialização do valor máximo ao cortar
    int maxValue = 0;

    // Loop para explorar todas as possibilidades de corte na linha
    for (int i = row - 1; i >= row / 2; i--) {
        // Atualiza o valor máximo ao cortar
        maxValue = max(maxValue, maxValues[i][col] + maxValues[row - i][col]);
    }

    // Loop para explorar todas as possibilidades de corte na coluna
    for (int i = col - 1; i >= col / 2; i--) {
        // Atualiza o valor máximo ao cortar
        maxValue = max(maxValue, maxValues[row][i] + maxValues[row][col - i]);
    }

    // Retorna o valor máximo ao cortar
    return maxValue;
}