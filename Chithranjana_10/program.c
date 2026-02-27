#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX 20

char production[MAX][MAX];
char first[MAX][MAX];
char follow[MAX][MAX];
char table[MAX][MAX][MAX];

int main() {
    int n, i, j, k;
    char nonTerminals[MAX];
    char terminals[MAX];
    int ntCount = 0, tCount = 0;

    printf("Enter number of productions: ");
    scanf("%d", &n);

    printf("Enter productions (Example: E=TR):\n");
    for(i = 0; i < n; i++) {
        scanf("%s", production[i]);
        
        // Take LHS only (before '=')
        if(isupper(production[i][0])) {
            nonTerminals[ntCount++] = production[i][0];
        }
    }

    // Remove duplicate non-terminals
    for(i = 0; i < ntCount; i++) {
        for(j = i + 1; j < ntCount; j++) {
            if(nonTerminals[i] == nonTerminals[j]) {
                for(k = j; k < ntCount; k++)
                    nonTerminals[k] = nonTerminals[k + 1];
                ntCount--;
                j--;
            }
        }
    }

    printf("\nEnter FIRST sets (without spaces, e.g., +i):\n");
    for(i = 0; i < ntCount; i++) {
        printf("FIRST(%c): ", nonTerminals[i]);
        scanf("%s", first[i]);
    }

    printf("\nEnter FOLLOW sets (without spaces, e.g., )$):\n");
    for(i = 0; i < ntCount; i++) {
        printf("FOLLOW(%c): ", nonTerminals[i]);
        scanf("%s", follow[i]);
    }

    // Extract terminals
    for(i = 0; i < n; i++) {
        for(j = 2; production[i][j] != '\0'; j++) {
            if(!isupper(production[i][j]) && production[i][j] != 'e') {
                terminals[tCount++] = production[i][j];
            }
        }
    }

    terminals[tCount++] = '$';

    // Remove duplicate terminals
    for(i = 0; i < tCount; i++) {
        for(j = i + 1; j < tCount; j++) {
            if(terminals[i] == terminals[j]) {
                for(k = j; k < tCount; k++)
                    terminals[k] = terminals[k + 1];
                tCount--;
                j--;
            }
        }
    }

    // Initialize table
    for(i = 0; i < ntCount; i++)
        for(j = 0; j < tCount; j++)
            strcpy(table[i][j], "");

    // Construct table
    for(i = 0; i < n; i++) {
        char lhs = production[i][0];
        char rhsFirst = production[i][2];

        int row;
        for(row = 0; row < ntCount; row++)
            if(nonTerminals[row] == lhs)
                break;

        if(rhsFirst == 'e') {
            for(j = 0; follow[row][j] != '\0'; j++)
                for(k = 0; k < tCount; k++)
                    if(terminals[k] == follow[row][j])
                        strcpy(table[row][k], production[i]);
        }
        else if(isupper(rhsFirst)) {
            int idx;
            for(idx = 0; idx < ntCount; idx++)
                if(nonTerminals[idx] == rhsFirst)
                    break;

            for(j = 0; first[idx][j] != '\0'; j++)
                for(k = 0; k < tCount; k++)
                    if(terminals[k] == first[idx][j])
                        strcpy(table[row][k], production[i]);
        }
        else {
            for(k = 0; k < tCount; k++)
                if(terminals[k] == rhsFirst)
                    strcpy(table[row][k], production[i]);
        }
    }

    // Display Table
    printf("\nLL(1) Predictive Parsing Table:\n\n");

    printf("     ");
    for(i = 0; i < tCount; i++)
        printf("%8c", terminals[i]);
    printf("\n");

    for(i = 0; i < ntCount; i++) {
        printf("%5c", nonTerminals[i]);
        for(j = 0; j < tCount; j++) {
            if(strlen(table[i][j]) == 0)
                printf("%8s", "-");
            else
                printf("%8s", table[i][j]);
        }
        printf("\n");
    }

    return 0;
}
