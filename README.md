# Maximum-Planner-Subset

Given is a set C of n chords of a circle (see Figure 1(a)). We assume that no two chords of C share an endpoint. Number the endpoints of these chords from 0 to 2n − 1, clockwise around the circle (see Figure 1(c)). Let 𝑀(𝑖, 𝑗), 𝑖 ≤ 𝑗, denote the number of chords in the maximum planar subset (i.e., no two chords overlap each other in the subset) in the region formed by the chord 𝑖𝑗 and the arc between the endpoints i and j (see Figure 1(d)). As the example shown in Figure 1(a), 𝑀(2,7) = 1, 𝑀(3,3) = 0, and 𝑀(0,11) = 3. You are asked to write a program that computes the number of chords in the maximum planar subset in a circle of n chords, i.e., compute 𝑀(0,2𝑛 − 1), and reports the details of each chords, as shown in Figure 1(b).

