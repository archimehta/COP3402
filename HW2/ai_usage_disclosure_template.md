# AI Usage Disclosure Details

**Student Name:** Archi Mehta & Ramyanaga Nagarur
**Student ID:** Ramyanaga: 5553091, Archi: 5552686
**Assignment:** Homework 2

---

## Instructions

Complete this template with detailed information about your AI usage. Submit this file along with your signed PDF declaration form.

---

## AI Tool #1

### Tool Name
ChatGPT

### Version/Model
GPT-5

### Date(s) Used
September 30, 2025

### Specific Parts of Assignment
AI was used to help write the correct logic for handling comments as an ignored element. More specifically, we used AI to help write the while loop that breaks after the comment block ends. 

### Prompts Used
After copying and pasting the while loop logic for ignoring comments that we initially wrote ourselves, we asked ChatGPT:
Is this loop properly ignoring the comment block during lexical analysis?

### AI Output/Results
The output was a refined version of the while loop that provided the correct if condition to break out of the loop when the comment ends, and uses a prev variable to track the current and prev char. The AI output also corrected a syntax error we made, likely due to a typo, where we were trying to compare a char to a string literal. 

### How Output was Verified/Edited
The output was verified by testing the code with some extra sample inputs/outputs created by another student and posted on the class Yellowdig forum. This sample input had multiple comment blocks in the Source Program, which allowed us to confirm whether the AI output code actually functioned correctly. 

### Multiple Iterations (if applicable)
Multiple iterations of prompts/dialogue were not necessary. 

### Learning & Reflection
AI assistance helped us understand how to track both the previous and current character to determine if the comment block ended. Using this understanding, we were able to write the correct break condition to stop reading the characters in the source program as part of a comment and instead continue the lexical analysis. 

## Overall Reflection

We had very minimal usage of AI for this assignment, since we were able to determine most of the logical conditions and loops ourselves. When trying to figure out the logic for ignoring commented sections, we got a little confused with all of the if, else, and while conditions, so AI helped us relieve some of that confusion. We still ensured that the work remained our own by using our variable names/types and modifying the loop to match our preferred style (AI gave us a do-while loop, we modified it into a while-loop).