# AI Usage Disclosure Details

**Student Name:** Archi Mehta & Ramyanaga Nagarur
**Student ID:** Archi: 5552686 & Ramyanaga: 5553091
**Assignment:** Homework 4

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
November 21, 2025

### Specific Parts of Assignment
AI was used to help troubleshoot and identify where our logical misunderstandings arose in the code. Since we were building on existing architecture, it was much more difficult to trace back the code and find where previous emits occurred. More specifically, we used AI to identify how to properly emit the CAL instruction and how to add a proc to the symbol table with the right address.  

### Prompts Used
We started off by feeding ChatGPT the pseudocode for the procedure-declaration and statement functions, and then I gave it the sample input from tokens.txt. We explained that we were having trouble getting the M-value for the CAL instruction to calculate/print as in the expected output. Then, we provided ChatGPT a code snippet from our procedure-declaration function that pertained to the error we were getting, asking it to explain how we should reorder our logic to meet the output requirements. 

We also asked AI to help us understand where our thought process/logic was wrong and resulting in an incorrect output. We wanted to make sure that we understood how the reordered code that it gave us was properly implementing the PL/0 parsing logic.

### AI Output/Results
AI helped us identify that we were emitting the instruction before properly calculating the jump index value, resulting in the incorrect terminal output. It provided us the code with the properly ordered function calls/instruction outputs. 

### How Output was Verified/Edited
The output was verified by implementing the AI-generated ordering/method for emitting and printing the instructions and symbol table. If the implementation was able to produce the expected output using the provided test cases + additional test cases, it was utilized in the code. 

### Multiple Iterations (if applicable)
A few iterations of prompts were necessary in order to make sure that the output implemented the correct logic for the PL/0 Machine ISA. The first few outputs that ChatGPT produced were not logically correct and when we tried using them in our code, it would change all of the offsets and instructions because it tried to emit extra instructions when that was not necessary. We refined and clarified our prompts by explaining to the AI how the correct flow theoretically should be, and asked it to implement that. 

### Learning & Reflection
AI assistance helped us identify the points of error that we had in the sequence of our logic. Since the parser code was already extensive, adding new logic to it for this assignment was slightly more convoluted and difficult to trace. Using AI allowed us to easily identify the problematic functions/lines of code, and then we were able to determine the correct logical flow to get the correct expected output. 
---

## Overall Reflection

AI was used with multiple iterations for this assignment, mainly because of how extensive and complex the parsing code was. With so much background and syntax that contributes to the parsing logic, it was difficult for AI to produce the correct solution to the problem we were facing since we only fed it the two funcitons we needed help with. However, we made sure that we explained the logic and how the addresses should actually work so that the AI could use that understanding and help us reflect it in the code. 

---

## Notes

- Be as specific and detailed as possible in your responses
- Include exact prompts and AI outputs when possible
- Explain how you verified and modified AI-generated content
- Reflect on what you learned through the AI interaction
