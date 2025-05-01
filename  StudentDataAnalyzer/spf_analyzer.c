/** @file route_manager.c
 *  @brief A small program to analyze airline routes data.
 *  @author Mike Z.
 *  @author Felipe R.
 *  @author Hausi M.
 *  @author Jose O.
 *  @author STUDENT_NAME
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "emalloc.h"

#define MAX_LINE_LEN 256
#define MAX_RECORDS 20


int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s --TASK=1, --TASK=2 or --TASK=3\n", argv[0]);
        return 1;
    }

    FILE *file = fopen("data/a3-data.csv", "r");
    if (!file) {
        perror("Error opening file");
        return 1;
    }

    char line[MAX_LINE_LEN];
    fgets(line, MAX_LINE_LEN, file); // Skip header row

    node_t *student_list = NULL;
    int count = 0;

    if (strcmp(argv[1], "--TASK=1") == 0) {
        while (fgets(line, MAX_LINE_LEN, file)) {
            int record_id, attendance, exam_score;
            char extracurricular[10];

            sscanf(line, "%d,%*d,%d,%9[^,],%*d,%*d,%*d,%d", &record_id, &attendance, extracurricular, &exam_score);

            if (attendance == 100 && strcmp(extracurricular, "Yes") == 0) {
                char formatted_entry[MAX_LINE_LEN];
                snprintf(formatted_entry, sizeof(formatted_entry), "%07d,%d", record_id, exam_score);

                node_t *new_student = new_node(formatted_entry);
                student_list = add_inorder(student_list, new_student);
                count++;
            }
        }
        fclose(file);

        FILE *output = fopen("output.csv", "w");
        if (!output) {
            perror("Error creating output file");
            return 1;
        }
        fprintf(output, "Record_ID,Exam_Score\n");

        node_t *curr = student_list;
        int written_count = 0;
        while (curr != NULL && written_count < MAX_RECORDS) {
            int record_id, exam_score;
            sscanf(curr->word, "%07d,%d", &record_id, &exam_score);
            fprintf(output, "%d,%d\n", record_id, exam_score);
            curr = curr->next;
            written_count++;
        }
        fclose(output);

    } else if (strcmp(argv[1], "--TASK=2") == 0) {
        while (fgets(line, MAX_LINE_LEN, file)) {
            int record_id, hours_studied, exam_score;

            sscanf(line, "%d,%d,%*d,%*[^,],%*d,%*d,%*d,%d", &record_id, &hours_studied, &exam_score);

            if (hours_studied > 40) {
                char formatted_entry[MAX_LINE_LEN];
                snprintf(formatted_entry, sizeof(formatted_entry), "%d,%07d,%d,%d", 999 - exam_score, record_id, hours_studied, exam_score);

                node_t *new_student = new_node(formatted_entry);
                student_list = add_inorder(student_list, new_student);
                count++;
            }
        }
        fclose(file);

        FILE *output = fopen("output.csv", "w");
        if (!output) {
            perror("Error creating output file");
            return 1;
        }
        fprintf(output, "Record_ID,Hours_Studied,Exam_Score\n");

        node_t *curr = student_list;
        int written_count = 0;
        while (curr != NULL && written_count < 10) {
            int record_id, hours_studied, exam_score;
            sscanf(curr->word, "%*d,%d,%d,%d", &record_id, &hours_studied, &exam_score);
            fprintf(output, "%d,%d,%d\n", record_id, hours_studied, exam_score);
            curr = curr->next;
            written_count++;
        }
        fclose(output);

    } else if (strcmp(argv[1], "--TASK=3") == 0) {
        while (fgets(line, MAX_LINE_LEN, file)) {
            int record_id, hours_studied, exam_score;

            sscanf(line, "%d,%d,%*d,%*[^,],%*d,%*d,%*d,%d", &record_id, &hours_studied, &exam_score);

            if (exam_score >= 85) {
                char formatted_entry[MAX_LINE_LEN];
                snprintf(formatted_entry, sizeof(formatted_entry), "%03d,%07d,%d,%d", exam_score, record_id, hours_studied, exam_score);

                node_t *new_student = new_node(formatted_entry);
                student_list = add_inorder(student_list, new_student);
                count++;
            }
        }
        fclose(file);

        FILE *output = fopen("output.csv", "w");
        if (!output) {
            perror("Error creating output file");
            return 1;
        }
        fprintf(output, "Record_ID,Hours_Studied,Exam_Score\n");

        node_t *curr = student_list;
        int written_count = 0;
        while (curr != NULL && written_count < 10) {
            int record_id, hours_studied, exam_score;
            sscanf(curr->word, "%*d,%d,%d,%d", &record_id, &hours_studied, &exam_score);
            fprintf(output, "%d,%d,%d\n", record_id, hours_studied, exam_score);
            curr = curr->next;
            written_count++;
        }
        fclose(output);
    }

    node_t *temp;
    while (student_list != NULL) {
        temp = student_list->next;
        free(student_list->word);
        free(student_list);
        student_list = temp;
    }

    printf("Task completed: output.csv generated.\n");
    return 0;
}
