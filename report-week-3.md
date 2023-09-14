# Repport of week 3
## Lexer (Tokenization)
- Write enum to store the type of token like ACTION, INTO, ID, ...
- Create a struct to store the token value and token type.
- Use regex library to extract token:
  + Extract keyword like INSERT, SELECT, INTO, WHERE, LIKE, ...
  + The name of the CSV file for example abc.csv, def.csv, ...
  + Phrases are in double quotes, words are separated by spaces, decimal numbers with periods, ...
  + But I still not handle when ID separated by comma. For example: (ID,NAME,AGE,EMAIL) -> "ID,NAME" and "AGE,EMAIL"
- Use if statement to give the token its type and them push it into the token vector.
## Parser (Syntactic Analysis)
My idea is:
- For INSERT statement:
  + I will break the command into 3 part which are name of the CSV file, list of column names (may or may not be available depending on user input) and list of values.
  + I found that there are something that must be exist in the statement:
    1. The phrases "INSERT INTO": it will be the first and second token in the token list. If not, it will be an error.
    2. The name of the CSV file whose token type is FILENAME. It must be the third token and again if not, it will be an error.
    3. The keyword "VALUES" whose position depends on whether there is a list of column names in front but it must be there in then command.
    4. Finally, the list of values.
    5. I still thinking how to handle the cases where the number of column names is greater than the number of values ​​and vice versa.
  + Then I write some funtion to open the file and insert the information as the command.
- For SELECT statement:
  + I still thinking right now and will update next week. Sorry for not being able to meet the set deadline.  
