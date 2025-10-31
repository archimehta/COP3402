# AI Usage Disclosure Details

**Student Name:** Archi Mehta & Ramyanaga Nagarur
**Student ID:** Archi: 5552686 & Ramyanaga: 5553091
**Assignment:** Homework 3

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
October 30, 2025

### Specific Parts of Assignment
AI was used very minimally for this assignment. The first part it was used for was to figure out why my JMP and JPC instruction were not printing the right M-value in the terminal/output file. The second thing it was used for was just to read my entire code and look for syntax errors, mispellings, and to check that none of the point penalties were present in my code.

### Prompts Used
Why is my output printing 7 0 1 when it should be printing 7 0 3? Why is my output printing 8 0 9 when it should be printing 8 0 24? Is it because I am not calculating my jump offset correctly?

Here is my completed code, it produces the correct output based on the test cases and it should be logically complete. Please  go through the code and check for syntax errors, mispellings, and make sure that none of the pasted penalties are present in the code. 

### AI Output/Results
AI explained to me that because my instructions are stored in a 1D array with OP, L, M, if I have to jump to a specific instruction, I have to multiply it by 3 to get the actual offset of that instruction from the base. 

AI provided a checklist for each function, making sure that there were no obvious typos or missing logical errors. It also created another checklist checking for penalties and making sure my code didn't have anything that would result in a penalty.

### How Output was Verified/Edited
Once AI helped me understand how the M offset value is actually calculated, I figured out that I needed to multiply the instrCodeIndex by 3 when emitting the instruction. This resulted in the correct output being printed. 

I utilized the checklist provided by AI to go through my code and double-check for syntax errors, misspellings, and potential penalties. My double-check aligned with the output AI provided. 

### Multiple Iterations (if applicable)
I did not need to use multiple iterations to produce a desired result/explanation.

### Learning & Reflection
AI assistance helped remind me that the offset M is calculated by multiplying the instrCodeIndex by 3 because of the 1D array nature of the instructions in this assignment. Though I knew this, I had forgotten, so it was helpful to get a reminder and therefore figure out why the output did not match with the expected output. 

---

## Overall Reflection

Once again, AI was used very minimally for this assignment. The HW3 instructions provided a very detailed and thorough guideline explaining the assignment and its requirements. The topics/logic used in this assignment was already very familiar, so it was just a matter of extending and applying it to the parser code. AI helped remind me about a logical assumption that we make when we work with a 1D instructions array, which allowed me to figure out why the output was wrong. AI was most beneficial for evaluating my completed code and being able to quickly look through the hundreds of lines to check for errors that could easily be missed by a tired human eye. AI was used very strictly as a supplemental research for questioning/verification - but not code generation in this assignment. 

---

## Notes

- Be as specific and detailed as possible in your responses
- Include exact prompts and AI outputs when possible
- Explain how you verified and modified AI-generated content
- Reflect on what you learned through the AI interaction
