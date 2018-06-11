\ Model Weights
\ LP format - for model browsing. Use MPS format to capture full model detail.
Maximize
  4.956 q1 + 11.841 q3 + 19.055 q5 + 29.572 q7 + 10.472 q9
Subject To
 R00: q0 = 0
 R01: q11 = 0
 R10: q1 - q2 = 0
 R11: q3 - q4 = 0
 R12: q5 - q6 = 0
 R13: q7 - q8 = 0
 R14: q9 - q10 = 0
 R20: q1 >= 0
 R21: q2 >= 0
 R22: q3 >= 0
 R23: q4 >= 0
 R24: q5 >= 0
 R25: q6 >= 0
 R26: q7 >= 0
 R27: q8 >= 0
 R28: q9 >= 0
 R29: q10 >= 0
 R30: q1 <= 12
 R31: q3 <= 12
 R32: q5 <= 8
 R33: q7 <= 6
 R34: q9 <= 16
 R40: Q0 + q1 - Q1 = 0
 R41: Q1 - q2 - Q2 = 0
 R42: Q2 + q3 - Q3 = 0
 R43: Q3 - q4 - Q4 = 0
 R44: Q4 + q5 - Q5 = 0
 R45: Q5 - q6 - Q6 = 0
 R46: Q6 + q7 - Q7 = 0
 R47: Q7 - q8 - Q8 = 0
 R48: Q8 + q9 - Q9 = 0
 R49: Q9 - q10 - Q10 = 0
 R410: Q10 - q11 - Q11 = 0
 R5: Q11 = 0
 R60: Q0 <= 15
 R61: Q1 <= 15
 R62: Q2 <= 15
 R63: Q3 <= 15
 R64: Q4 <= 15
 R65: Q5 <= 15
 R66: Q6 <= 15
 R67: Q7 <= 15
 R68: Q8 <= 15
 R69: Q9 <= 15
 R610: Q10 <= 15
 R611: Q11 <= 15
 R70: Q0 >= 0
 R71: Q1 >= 0
 R72: Q2 >= 0
 R73: Q3 >= 0
 R74: Q4 >= 0
 R75: Q5 >= 0
 R76: Q6 >= 0
 R77: Q7 >= 0
 R78: Q8 >= 0
 R79: Q9 >= 0
 R710: Q10 >= 0
 R711: Q11 >= 0
Bounds
End
