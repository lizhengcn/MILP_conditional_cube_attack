
# MILP_conditional_cube_attack
For Keccak-MAC-384, whose capacity is 768-bit.
1. Using SageMath, run file "KeccakMAC384c768MILPgenlp.py" and obtain file "lpKeccakMAC384c768.lp" as a model in Gurobi.
2. Using Gurobi, read and optimize the model file "lpKeccakMAC384c768.lp", then obtain the result file "solKeccakMAC384c768.sol", i.e. the maximum number of cube variables all in CP-kernel is 66.

For Keccak-MAC-512, whose capacity is 1024-bit.
1. Program "Test_on_6round_Keccak_MAC_512_rightkey.cpp" test whether the cube sum with the right key guess is zero or not.
2. Program "Test_on_6round_Keccak_MAC_512_randomkey.cpp" test whether the cube sum with a random key guess(wrong key guess in common cases) is zero or not.

For Ketje Major, we list the details and source code in the master "Attacks_on_reduced-round_KetjeMajor".
1. Document "Details_of_attacks_on_reduced-round_KetjeMajor.pdf" decribes the details of 6/7-round attacks on Ketje Major with 960/768-bit padding, especially the parameters sets.

For Ketje Major with 960-bit padding。
2. Program "Test_on_6round_KetjeMajorPadding960_rightkey.cpp" test whether the cube sum with the right key guess is zero or not.  
3. Program "Test_on_6round_KetjeMajorPadding960_randomkey.cpp" test whether the cube sum with a random key guess(wrong key guess in common cases) is zero or not. 
