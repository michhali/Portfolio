#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on Jan 28 10:04:26 2025
Based on: https://www.kaggle.com/datasets/lainguyn123/student-performance-factors
Sample input: --TASK="1", --TASK="2", or --TASK="3"
@author: rivera
@author: STUDENT_ID
"""

import pandas as pd
import argparse
from typing import List, Tuple, NoReturn

def read_csv_file(filename: str) -> pd.DataFrame:
    """Reads a CSV file and extracts relevant student data as a DataFrame.
    
    Parameters
    ----------
    filename : str
        The path to the CSV file.
    
    Returns
    -------
    pd.DataFrame
        A DataFrame containing student data.
    """
    try:
        df = pd.read_csv(filename)
        return df
    except FileNotFoundError:
        print(f"Error: Could not open file {filename}")
        return pd.DataFrame()
    except Exception as e:
        print(f"An error occurred while reading the file: {e}")
        return pd.DataFrame()

def filter_students(input_file: str, output_file: str) -> NoReturn:
    """Filters students who studied more than 40 hours and saves the result to a CSV file.
    
    Parameters
    ----------
    input_file : str
        The path to the input CSV file.
    output_file : str
        The path to save the filtered CSV file.
    """
    df = read_csv_file(input_file)
    filtered_df = df[df["Hours_Studied"] > 40][["Record_ID", "Hours_Studied", "Exam_Score"]]
    filtered_df.to_csv(output_file, index=False)
    print(f"Filtered data saved to {output_file}")

def top_students(input_file: str, output_file: str) -> NoReturn:
    """Generates a CSV with the top 10 students who scored at least 85, sorted by Exam_Score desc, then Record_ID asc.
    
    Parameters
    ----------
    input_file : str
        The path to the input CSV file.
    output_file : str
        The path to save the filtered CSV file.
    """
    df = read_csv_file(input_file)
    filtered_df = df[df["Exam_Score"] >= 85][["Record_ID", "Hours_Studied", "Exam_Score"]]
    filtered_df = filtered_df.sort_values(by=["Exam_Score", "Record_ID"], ascending=[False, True]).head(10)
    filtered_df.to_csv(output_file, index=False)
    print(f"Top 10 students data saved to {output_file}")

def filter_attendance_extracurricular(input_file: str, output_file: str) -> NoReturn:
    """Generates a CSV containing Record_ID and Exam_Score for students with 100% attendance and extracurricular activities.
    
    Parameters
    ----------
    input_file : str
        The path to the input CSV file.
    output_file : str
        The path to save the filtered CSV file.
    """
    df = read_csv_file(input_file)
    filtered_df = df[(df["Attendance"] == 100) & (df["Extracurricular_Activities"] == "Yes")][["Record_ID", "Exam_Score"]]
    filtered_df.to_csv(output_file, index=False)
    print(f"Filtered data saved to {output_file}")

def compute_average_attendance(input_file: str, output_file: str) -> NoReturn:
    """Generates a CSV containing Grade categories and corresponding Average Attendance."""
    df = read_csv_file(input_file)
    
    grade_bins = [0, 49, 59, 64, 69, 72, 76, 79, 84, 89, 101]  # Changed 100 → 101

    grade_labels = ["F", "D", "C", "C+", "B-", "B", "B+", "A-", "A", "A+"]
    
    df["Grade"] = pd.cut(df["Exam_Score"], bins=grade_bins, labels=grade_labels, include_lowest=True)
    df.loc[df["Exam_Score"] > 100, "Grade"] = "A+"
    avg_attendance = df.groupby("Grade", observed=False)["Attendance"].agg(lambda x: round(x.mean(), 1)).reset_index()
    avg_attendance = avg_attendance.set_index("Grade").reindex(grade_labels[::-1]).reset_index()
    avg_attendance = avg_attendance.dropna(subset=["Attendance"])
    avg_attendance.to_csv(output_file, index=False, header=True)
    print(f"Average attendance data saved to {output_file}")

def compute_tutoring_analysis(input_file: str, output_file: str) -> NoReturn:
    """Generates a CSV analyzing tutoring sessions per grade group."""
    df = read_csv_file(input_file)
    
    grade_bins = [0, 49, 59, 69, 79, 100]  # Includes 101 for outliers
    grade_labels = ["F", "D", "C", "B", "A"]
    
    df["Grade"] = pd.cut(df["Exam_Score"], bins=grade_bins, labels=grade_labels, include_lowest=True)
    df.loc[df["Exam_Score"] > 100, "Grade"] = "A"  # Assign outlier to 'A'

    avg_tutoring = df.groupby("Grade", observed=False)["Tutoring_Sessions"].mean().round(1).reset_index()
    df = df.merge(avg_tutoring, on="Grade", suffixes=(None, "_Average"))
    df.rename(columns={"Tutoring_Sessions_Average": "Grade_Average_Tutoring_Sessions"}, inplace=True)
    df["Above_Average"] = df["Tutoring_Sessions"] > df["Grade_Average_Tutoring_Sessions"]
    
    output_df = df[["Record_ID", "Tutoring_Sessions", "Grade_Average_Tutoring_Sessions", "Above_Average", "Exam_Score", "Grade"]]
    output_df = output_df.sort_values(by=["Exam_Score", "Record_ID"], ascending=[False, True]).head(50)
    
    output_df.to_csv(output_file, index=False, header=True)
    print(f"Tutoring analysis data saved to {output_file}")

   

def parse_arguments() -> argparse.Namespace:
    """Parses command-line arguments.
    
    Returns
    -------
    argparse.Namespace
        Parsed command-line arguments.
    """
    parser = argparse.ArgumentParser()
    parser.add_argument("--TASK", type=str, required=True, help="Task number to execute")
    return parser.parse_args()

def main() -> NoReturn:
    """Main entry point of the program."""
    args = parse_arguments()
    
    input_file = "./data/a2-data.csv"
    output_file = "output.csv"
    
    if args.TASK == "1":
        filter_students(input_file, output_file)
    elif args.TASK == "2":
        top_students(input_file, output_file)
    elif args.TASK == "3":
        filter_attendance_extracurricular(input_file, output_file)
    elif args.TASK == "4":
        compute_average_attendance(input_file, output_file)
    elif args.TASK == "5":
        compute_tutoring_analysis(input_file, output_file)
    else:
        print("Invalid task number. Supported tasks: 1, 2, 3")

if __name__ == '__main__':
    main()
