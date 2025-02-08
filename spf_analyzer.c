#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 6608

typedef struct {
    int record_id;
    int hours_studied;
    int attendance;
    int tutoring_sessions;
    int exam_score;
} CurricularData;

typedef struct {
    int record_id;
    char extracurricular_activities[256];
    int physical_activity;
    int sleep_hours;
} ExtracurricularData;

int read_csv_file(const char *filename, CurricularData data[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return -1;
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), file);

    int index = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL && index < MAX_RECORDS) {
        sscanf(buffer, "%d,%d,%d,%d,%d",
               &data[index].record_id,
               &data[index].hours_studied,
               &data[index].attendance,
               &data[index].tutoring_sessions,
               &data[index].exam_score);
        index++;
    }

    fclose(file);
    return index;
}

// Function to write curricular records with exam scores >= 90 to a CSV file
void write_csv_file(const char *filename, CurricularData data[], int size) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file %s for writing\n", filename);
        return;
    }

    fprintf(file, "Record_ID,Exam_Score\n");
    for (int i = 0; i < size; i++) {
        if (data[i].exam_score >= 90) {
            fprintf(file, "%d,%d\n", data[i].record_id, data[i].exam_score);
        }
    }

    fclose(file);
    printf("Successfully wrote curricular records to %s\n", filename);
}

// Function to read the extracurricular YAML file
int read_yaml_file(const char *filename, ExtracurricularData data[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return -1;
    }

    char buffer[256];
    int index = -1;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strncmp(buffer, "records:", 8) == 0) {
            continue;
        }
        if (strstr(buffer, "-")) {
            index++;
            memset(&data[index], 0, sizeof(ExtracurricularData));
        }
        if (strstr(buffer, "Record_ID:")) {
            sscanf(buffer, " Record_ID: %d", &data[index].record_id);
        } else if (strstr(buffer, "Extracurricular_Activities:")) {
            char *start = strstr(buffer, ": '") + 3;
            start[strcspn(start, "'\n")] = '\0';
            strcpy(data[index].extracurricular_activities, start);
        } else if (strstr(buffer, "Physical_Activity:")) {
            sscanf(buffer, " Physical_Activity: %d", &data[index].physical_activity);
        } else if (strstr(buffer, "Sleep_Hours:")) {
            sscanf(buffer, " Sleep_Hours: %d", &data[index].sleep_hours);
        }
    }

    fclose(file);
    return index + 1;
}

void write_below_60_csv(const char *filename, CurricularData curr_data[], ExtracurricularData extra_data[], int curr_size, int extra_size) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file %s for writing\n", filename);
        return;
    }

    fprintf(file, "Record_ID,Exam_Score,Extracurricular_Activities\n");
    for (int i = 0; i < curr_size; i++) {
        if (curr_data[i].exam_score < 60) {
            for (int j = 0; j < extra_size; j++) {
                if (curr_data[i].record_id == extra_data[j].record_id) {
                    fprintf(file, "%d,%d,%s\n",
                            curr_data[i].record_id,
                            curr_data[i].exam_score,
                            extra_data[j].extracurricular_activities);
                    break;
                }
            }
        }
    }

    fclose(file);
    printf("Successfully wrote records with Exam_Score < 60 to %s\n", filename);
}

void write_extracurricular_csv(const char *filename, ExtracurricularData data[], int size) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file %s for writing\n", filename);
        return;
    }

    fprintf(file, "Extracurricular_Activities,Physical_Activity,Record_ID,Sleep_Hours\n");
    for (int i = 0; i < size; i++) {
        fprintf(file, "%s,%d,%d,%d\n",
                data[i].extracurricular_activities,
                data[i].physical_activity,
                data[i].record_id,
                data[i].sleep_hours);
    }

    fclose(file);
    printf("Successfully wrote extracurricular records to %s\n", filename);
}

void write_attendance_csv(const char *filename, CurricularData data[], int size) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file %s for writing\n", filename);
        return;
    }

    fprintf(file, "Record_ID,Exam_Score\n");
    for (int i = 0; i < size; i++) {
        if (data[i].attendance == 100) {
            fprintf(file, "%d,%d\n", data[i].record_id, data[i].exam_score);
        }
    }

    fclose(file);
    printf("Successfully wrote 100%% attendance records to %s\n", filename);
}

void write_sleep_vs_study_csv(const char *filename, CurricularData curr_data[], ExtracurricularData extra_data[], int curr_size, int extra_size) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file %s for writing\n", filename);
        return;
    }

    fprintf(file, "Record_ID,Exam_Score\n");
    for (int i = 0; i < curr_size; i++) {
        for (int j = 0; j < extra_size; j++) {
            if (curr_data[i].record_id == extra_data[j].record_id && extra_data[j].sleep_hours >= curr_data[i].hours_studied) {
                fprintf(file, "%d,%d\n", curr_data[i].record_id, curr_data[i].exam_score);
                break;
            }
        }
    }

    fclose(file);
    printf("Successfully wrote records with Sleep_Hours >= Hours_Studied to %s\n", filename);
}

void write_merged_csv(const char *filename, CurricularData curricular_data[], int curricular_size, ExtracurricularData extracurricular_data[], int extracurricular_size) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not open file %s for writing\n", filename);
        return;
    }

    fprintf(file, "Record_ID,Exam_Score,Hours_Studied,Attendance,Tutoring_Sessions,Extracurricular_Activities,Physical_Activity,Sleep_Hours\n");
    for (int i = 0; i < curricular_size; i++) {
        if (curricular_data[i].exam_score > 90) {
            int found = 0;
            for (int j = 0; j < extracurricular_size; j++) {
                if (curricular_data[i].record_id == extracurricular_data[j].record_id) {
                    fprintf(file, "%d,%d,%d,%d,%d,%s,%d,%d\n",
                            curricular_data[i].record_id,
                            curricular_data[i].exam_score,
                            curricular_data[i].hours_studied,
                            curricular_data[i].attendance,
                            curricular_data[i].tutoring_sessions,
                            extracurricular_data[j].extracurricular_activities,
                            extracurricular_data[j].physical_activity,
                            extracurricular_data[j].sleep_hours);
                    found = 1;
                    break;
                }
            }
            if (!found) {
                fprintf(file, "%d,%d,%d,%d,%d,,,\n",
                        curricular_data[i].record_id,
                        curricular_data[i].exam_score,
                        curricular_data[i].hours_studied,
                        curricular_data[i].attendance,
                        curricular_data[i].tutoring_sessions);
            }
        }
    }

    fclose(file);
    printf("Successfully wrote merged records to %s\n", filename);
}


                      
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s --TASK=\"<task_number>\"\n", argv[0]);
        return 1;
    }

    int task_id = atoi(argv[1] + 7);

    CurricularData curricular_data[MAX_RECORDS];
    ExtracurricularData extracurricular_data[MAX_RECORDS];
    const char *curricular_filename = "data/a1-data-curricular.csv";
    const char *extracurricular_filename = "data/a1-data-extracurricular.yaml";
    const char *output_filename = "output.csv";

    if (task_id == 1) {
        int num_curricular_records = read_csv_file(curricular_filename, curricular_data);
        if (num_curricular_records < 0) {
            return 1;
        }
        write_csv_file(output_filename, curricular_data, num_curricular_records);
    } else if (task_id == 2) {
        int num_extracurricular_records = read_yaml_file(extracurricular_filename, extracurricular_data);
        if (num_extracurricular_records < 0) {
            return 1;
        }
        write_extracurricular_csv(output_filename, extracurricular_data, num_extracurricular_records);
    } else if (task_id == 3) {
        int num_curricular_records = read_csv_file(curricular_filename, curricular_data);
        int num_extracurricular_records = read_yaml_file(extracurricular_filename, extracurricular_data);
        if (num_curricular_records < 0 || num_extracurricular_records < 0) {
            return 1;
        }
        write_merged_csv(output_filename, curricular_data, num_curricular_records, extracurricular_data, num_extracurricular_records);
    } else if (task_id == 4) {
        int num_curricular_records = read_csv_file(curricular_filename, curricular_data);
        if (num_curricular_records < 0) {
            return 1;
        }
        write_attendance_csv(output_filename, curricular_data, num_curricular_records);
    } else if (task_id == 5) {
        int num_curricular_records = read_csv_file(curricular_filename, curricular_data);
        int num_extracurricular_records = read_yaml_file(extracurricular_filename, extracurricular_data);
        if (num_curricular_records < 0 || num_extracurricular_records < 0) {
            return 1;
        }
        write_sleep_vs_study_csv(output_filename, curricular_data, extracurricular_data, num_curricular_records, num_extracurricular_records);
    } else if (task_id == 6) {
        int num_curricular_records = read_csv_file(curricular_filename, curricular_data);
        int num_extracurricular_records = read_yaml_file(extracurricular_filename, extracurricular_data);
        if (num_curricular_records < 0 || num_extracurricular_records < 0) {
            return 1;
        }
        write_below_60_csv(output_filename, curricular_data, extracurricular_data, num_curricular_records, num_extracurricular_records);
    }

    return 0;
}

