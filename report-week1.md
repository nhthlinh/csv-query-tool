# Report of week 1

## Set up

1. Environment:
  - Install Ubuntu (22.04)
  - Git and Github
  - Install VSCode as my text editor
  - Install Valgrind
2. Set up repository on Github to store solution for this challenge.
3. Watch some SQLite video on Youtube.
4. Read the challenge described carefully.

## Idea

1. Command-line interface
  - Check if the command-line arguments is corect.
  - Handle incorect input.
2. Query parsing
  - Implement a funtion to parse command-line arguments (include CSV file name and quuery).
  - Develop a query parser that can recognize SELECT and INSERT statements.
  - Extract the necessary information such as CSV file name, data to insert, columns to select,...
3. CSV Reading
  - Write a funtion to read and store data in the CSV file
  - Store data in a structure like arrays, linked list,...
  - Handle the header rows to display the data in table with name of the data such as id, first_name,...
4. Query Execution
  - Process SELECT and INSERT query
  - SELECT: retrive the specified columns of data bases on the statement.
  - INSERT: add new data to the CSV file.