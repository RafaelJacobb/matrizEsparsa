#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
typedef struct matrix {
  struct matrix *right;
  struct matrix *below;
  int line;
  int column;
  float info;
} Matrix;

typedef struct matrix Matrix;

void insertCell(Matrix *head, int line, int column, float value);
Matrix *createCell(int line, int column, float value);
Matrix *createHead();
Matrix *matrix_create(int m, int n, float values[m][n]);
void matrix_destroy(Matrix *m);
void matrix_print(Matrix *m);
Matrix *matrix_add(Matrix *m, Matrix *n);
Matrix *matrix_multiply(Matrix *m, Matrix *n);
Matrix *matrix_transpose(Matrix *m);
float matrix_getelem(Matrix *m, int x, int y);
void matrix_setelem(Matrix *m, int x, int y, float elem);

Matrix *matrix_create(int m, int n, float values[m][n]) {
  // Aloca memória para a estrutura de matriz
  Matrix *mat = (Matrix *)malloc(sizeof(Matrix));
  mat->right = mat->below = NULL;
  mat->line = m;
  mat->column = n;

  // Preenche a matriz com os valores fornecidos
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      float value = values[i][j];
      
      // Aloca memória para o nó da matriz
      Matrix *node = (Matrix *)malloc(sizeof(Matrix));
      node->right = node->below = NULL;
      node->line = i;
      node->column = j;
      node->info = value;

      // Insere o nó na linha da matriz
      Matrix *row = mat;
      while (row->right != NULL && row->right->column < j)
        row = row->right;
      node->right = row->right;
      row->right = node;

      // Insere o nó na coluna da matriz
      Matrix *col = mat;
      while (col->below != NULL && col->below->line < i)
        col = col->below;
      node->below = col->below;
      col->below = node;
    }
  }
  
  // Retorna a matriz criada
  return mat;
}

void matrix_destroy(Matrix *m) {
    Matrix* currentLine = m->below;
    while (currentLine != m) {
        Matrix* current = currentLine->right;
        while (current != currentLine) {
            Matrix* temp = current;
            current = current->right;
            free(temp);
        }
        Matrix* tempLine = currentLine;
        currentLine = currentLine->below;
        free(tempLine);
    }

   
    free(m); // Free the main header node
}
void matrix_print(Matrix *m) {
  printf("________________\n");
  for (int i = 0; i < m->line; i++) {
    for (int j = 0; j < m->column; j++) {
      if (matrix_getelem(m, i, j) != 0.0) {
        printf("%d %d %.2f\n", i, j, matrix_getelem(m, i, j));
      }
    }
  }
}
Matrix *matrix_add(Matrix *m, Matrix *n) {
  if (m == NULL || n == NULL || m->line != n->line || m->column != n->column) {
    return NULL;
  }

  Matrix *result = createHead();
  result->line = m->line;
  result->column = m->column;

  for (int x = 0; x < m->line; x++) {
    for (int y = 0; y < m->column; y++) {
      float sum = matrix_getelem(m, x, y) + matrix_getelem(n, x, y);
      if (sum != 0.0) {
        matrix_setelem(result, x, y, sum);
      }
    }
  }

  return result;
}

Matrix *createCell(int line, int column, float value) {
  Matrix *cell = (Matrix *)malloc(sizeof(Matrix));
  cell->right = NULL;
  cell->below = NULL;
  cell->line = line;
  cell->column = column;
  cell->info = value;
  return cell;
}

void insertCell(Matrix *head, int line, int column, float value) {
  Matrix *newCell = createCell(line, column, value);

  Matrix *currentLine = head;
  while (currentLine->below != head && currentLine->below->line < line) {
    currentLine = currentLine->below;
  }

  Matrix *currentColumn = head;
  while (currentColumn->right != head &&
         currentColumn->right->column < column) {
    currentColumn = currentColumn->right;
  }

  // Inserir a nova célula na linha correta
  newCell->right = currentLine->right;
  currentLine->right = newCell;

  // Inserir a nova célula na coluna correta
  newCell->below = currentColumn->below;
  currentColumn->below = newCell;
}

Matrix *createHead() {
  Matrix *head = (Matrix *)malloc(sizeof(Matrix));
  head->right = head;
  head->below = head;
  head->line = -1;   // Marcando como cabeçalho
  head->column = -1; // Marcando como cabeçalho
  head->info = 0;    // Não usado para cabeçalhos
  return head;
}

Matrix *matrix_multiply(Matrix *m, Matrix *n) {
  if (m == NULL || n == NULL || m->line != n->line || m->column != n->column) {
    return NULL;
  }

  Matrix *result = createHead();
  result->line = m->line;
  result->column = m->column;
  for (int i = 0; i < m->line; i++) {
    for (int j = 0; j < n->column; j++) {
      float sum = 0.0;
      for (int k = 0; k < m->column; k++) {
        sum += matrix_getelem(m, i, k) * matrix_getelem(n, k, j);
      }
      matrix_setelem(result, i, j, sum);
    }
  }

  return result;
}

Matrix *matrix_transpose(Matrix *m) {
  if (m == NULL) {
    return NULL;
  }

  Matrix *result = createHead();
  result->line = m->column;
  result->column = m->line;

  for (int x = 0; x < m->line; x++) {
    for (int y = 0; y < m->column; y++) {
      float value = matrix_getelem(m, x, y);
      matrix_setelem(result, y, x, value);
    }
  }

  return result;
}

float matrix_getelem(Matrix *m, int x, int y) {
  if (m == NULL || x < 0 || y < 0 || x >= m->line || y >= m->column) {
    return 0.0;// Entrada inválida ou fora dos limites
  }

  for (Matrix *cell = m->right; cell != m; cell = cell->right) {
    if (cell->line == x && cell->column == y) {
      return cell->info;
    }
  }

  return 0.0;
}

void matrix_setelem(Matrix *m, int x, int y, float elem) {
  if (m == NULL || x < 0 || y < 0 || x >= m->line || y >= m->column) {
    return; // Entrada inválida ou fora dos limites
  }

  // Procurar pela célula na coluna correspondente
  Matrix *colHeader = m->below;
  while (colHeader != m && colHeader->line <= x) {
    colHeader = colHeader->below;
  }

  Matrix *prevCell = colHeader;
  Matrix *cell = colHeader->right;
  while (cell != colHeader && cell->column <= y) {
    prevCell = cell;
    cell = cell->right;
  }

  if (cell != colHeader && cell->column == y) {
    // A célula já existe, atualize seu valor
    cell->info = elem;
  } else {
    // A célula não existe, insira uma nova
    insertCell(prevCell, x, y, elem);
  }
}

#endif