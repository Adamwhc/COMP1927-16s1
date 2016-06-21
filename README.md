
[Source](https://www.cse.unsw.edu.au/~cs1927/16s1/ass/ass02/Ass2.html "Permalink to COMP1927 16s1 - Assignment 2")

# COMP1927 16s1 - Assignment 2

## COMP1927: Assignment 2

## Simple Search Engines

#### (Draft v8, Thursday 02 June)

** This is an initial draft, and it may change! A notice on the class web page will be posted after each revision, so please check class notice board frequently.

#### Change log / Notes :

* (2 June) **Section 1A: Sample files** (with expected pagerankList.txt) are added in Section 1A. Please read associated instructions (marked by lightyellow background colour), before comparing your results, thanks.
* (2 June) **Part-3, now has minimum distance** for the example, along with expected output, (marked by lightyellow background colour).
* (31 May) Instructions on how to submit your Ass2 added in the "Submission" section.
* (31 May) Due time is 23:59:59 (was 5pm earlier) on 03 Friday.
* (30 May) Late penalty is now reduced to 0.25 mark per day, off the ceiling.
* (26 May) Typo: "#end Setion-2" changed to "#end Section-2". The sample files are also corrected accordingly, so please download them again, thanks.
* (25 May) **Last day to submit** this assignment is 11pm Friday 10 Jun, subject to late penalty of <s>0.5</s> 0.25 mark (now reduced from 1 to <s>0.5</s> 0.25 mark) per day off the ceiling.
* (24 May) Added more description on possible P values in Part-3
* (24 May) Added reference to the presentation on topic "Rank aggregation method for the web", in the firsr pararaph of Part-3.
* (24 May) Typo, file name `searchTdidf.c` is changed to `searchTfIdf.c`; also corresponding command name
* (24 May) Your program `searchTfIdf.c` must also output the corresponding summation of tf-idf along with each page, separated by a space and using format "%.6f", see example below
* (24 May) Part-3: Hybrid Search Engine using Rank Aggregation **added**
* (20 May) pagerankList.txt now has consistent spacing; values are comma separated, and one space after comma.
* (20 May) PageRank algorithm is "slightly" revised. Earlier, diff was set to zero before while loop, so the loop was not entered for any diffPR &gt; 0 (say 0.00001)! Also, diff is reset to zero for each iteration. Tutor/Student pointed this out, thanks! I have also added end of loop lines for more readability.
* (20 May) In part-2, instructions added on how to calculate **tf** and **idf** values.
* (18 May) File pagerank_list.txt is renamed to pagerankList.txt
* Sample1.zip file is available, see after the section "Aim".
* You can use code from labs/lecture material, however, must acknowledge it and provide a reference. For example you can use graph ADT implementation from one of the labs, and adapt it for this assignment.
* You can build a graph structure using Adjacency Matrix or List Representation.
* Read lecture notes from Wed 18 May, on [** "How to Implement Ass2 Part-1" ][1] **
* For this assignment, you can assume that a url string will not be longer than 50 characters.
* There is no limit on number of urls or words, you should use dynamic data structures like lists, trees, etc. If you need to use an array, calculate possible size for the required memory allocation. For example, you need to count say number of urls from file "collection.txt", and accordingly create an array.

## Objectives

* to implement simple search engines using well known algorithms like PageRank and tf-idf (simplified version for this assignment!)
* to give you further practice with C and data structures (Graph ADT)
* to give you experience working in a team

## Admin

| ----- |
| **Marks** |  15 marks (14 marks towards total course mark)  |
| **Group** |  This assignment is completed in **group of two** |
| **Due** |  Fri Jun 3 23:59:59 2016. Last day to submit** this assignment is 11pm Friday 10 Jun, subject to late penalty of 0.25 mark per day off the ceiling. |
| **Submit** |  Read instructions in the "Submission" section below, and follow the submission link.  |
| **Late Penalty** | 0.25 mark per day off the ceiling. Last day to submit this assignment is 11pm Friday 10 June.|

## Aim

In this assignment, your task is to implement simple search engines using well known algorithms like PageRank and tf-idf, simplified for this assignment, of course!. You should start by reading the wikipedia entries on these topics. I will also discuss these topics in the lecture.

The main focus of this assignment is to build a graph structure, calculate PageRank, tf-idf, etc. and rank pages based one these values. You don't need to spend time crawling, collecting and parsing weblinks for this assignment! You will be provided with a collection of "web pages" with the required information for this assignment in a easy to use format. For example, each page has two sections,

* Section-1 contains urls representing outgoing links. Urls are separated by one or more blanks, across multiple lines.
* Section-2 contains selected words extracted from the url. Words are separated by one or more spaces, spread across multiple lines.

Example file `url31.txt`

        #start Section-1

        url2  url34  url1 url26
        url52 url21
        url74  url6 url82

        #end Section-1

        #start Section-2

        Mars has long been the subject of human interest. Early telescopic observations
        revealed color changes on the surface that were attributed to seasonal vegetation
        and apparent linear features were ascribed to intelligent design.

        #end Section-2

In Part-1: Graph structure-based search engine, you need to create a graph structure that represents a hyperlink structure of given collection of "web pages" and for each page (node in your graph) calculate PageRank and other graph properties. You need to create "inverted index" that provides a list of pages for every word in a given collection of pages. Your graph-structure based search engine will use this inverted index to find pages where query term(s) appear and rank these pages using their PageRank values.

In Part-2: Content-based search engine, you need to calculate tf-idf values for each query term in a page, and rank pages based on the summation of tf-idf values for all query terms. Use "inverted index" you created in Part-1 to locate matching pages for query terms.

In Part-3: Hybrid search engine, you need to combine both PageRank and tf-idf values in order to rank pages. More on this later ...

**Additional files:** You can submit additional supporting files, `*.c`and `*.h`, for this assignment. For example, you may implement your graph adt in files `graph.c` and `graph.h` and submit these two files along with other required files as mentioned below.
* * *

##  Sample files

* * *

## Part-1: Graph structure-based Search Engine

(8 marks)

### A: Calculate PageRanks

You need to write a program in the file `pagerank.c` that reads data from a given collection of pages in the file `collection.txt` and builds a graph structure using Adjacency Matrix or List Representation. Using the algorithm described below, calculate PageRank for every url in the file `collection.txt`. In this file, urls are separated by one or more spaces or/and new line character. Add suffix `.txt` to a url to obtain file name of the corresponding "web page". For example, file `url24.txt` contains the required information for `url24`.

Example file `collection.txt`

        url25   url31 url2
           url102   url78
        url32  url98 url33

Simplified PageRank Algorithm (for this assignment)

            **PageRank(d, diffPR, maxIterations)**

                Read "web pages" from the collection in file "collection.txt"
                and build a graph structure using Adjacency List Representation

                N = number of urls in the collection
                For each url _pi_ in the collection
                    PR(pi) = 1/N  ;
    	    End For

                iteration = 0;
                diff = diffPR;   // to enter the following loop

                While (iteration = diffPR)
                    iteration++;
    	        diff = 0;    // reset diff for each iteration

                    For each url _pi_ in the collection

                        PR_old = PR(pi);
                        sum = 0 ;

                        For each url _pj_ pointing to _pi_ (ignore self-loops and parallel edges)
                                  sum = sum + PR(pj) / out-degree-of(pj);
    	            End For

                        PR(pi) = (1-d)/N + d * sum
    	            diff = diff + Abs(PR_old - PR(pi);

    	        End For

    	     End While

Your program in `pagerank.c` will take three arguments (**d** \- damping factor, **diffPR** \- difference in PageRank sum, **maxIterations** \- maximum iterations) and using the algorithm described in this section, calculate PageRank for every url.

For example,

        % pagerank 0.85  0.00001  1000

Your program should output a list of urls in descending order of PageRank values (to 8 significant digits) to a file named `pagerankList.txt`. The list should also include out degrees (number of out going links) for each url, along with its PageRank value. The values in the list should be comma separated. For example, `pagerankList.txt` may contain the following:

Example file `pagerankList.txt`

        url102, 137, .0025881
        url4, 196, .0023054
        url75, 7, .0021345

#### Sample Files for 1A

You can download the following three sample files with expected pagerankList.txt files. Please note that your pagerank values might be slightly different to that provided in these samples. This might be due to the way you carried out calculations or different interpretation of the algorithm. As discussed in the lecture, there many ways to implement pagerank algorithm. For this assignment, I provided "simplified" algorithm for you to implement. However, in case you implemented more detailed algorithm, do not worry, that's good! All you need to check is that your pagerank values match to say first 3 decimal points to the expected values. In case this is not the case, and you believe your algorithm is correct! Please document your algorithm and let me know. In Summary, say an expected value is 0.1843066, your value could be 0.184xxxx where x could be any digit.

Sample files for 1A, all the sample files were generated using the following command:

    % pagerank  0.85  0.00001  1000

### B: Inverted Index

You need to write a program in the file named `inverted.c` that reads data from a given collection of pages in `collection.txt` and generates an "inverted index" that provides a list (set) of urls for every word in a given collection of pages. You need to "normalise" words by removing leading and trailing spaces and converting all characters to lowercases before inserting words in your index. In each list (set), duplicate urls are not allowed. Your program should output this "inverted index" to a file named `invertedIndex.txt`. The list should be alphabetically ordered, using ascending order.

Example file `invertedIndex.txt`

        design  url31 url2 url61 url25
        mars url31 url25 url101
        vegetation  url61 url31

### C: Search Engine

Write a simple search engine in file `searchPagerank.c` that given search terms (words) as commandline arguments, outputs (to stdout) top ten pages in descending order of PageRank. If number of matches are less than 10, output all of them. Your program must use data available in two files `invertedIndex.txt` and `pagerankList.txt`, and must derive result from them. We will test this program independently to your solutions for "A" and "B".

Example:

        % searchPagerank  mars  design
        url31
        url25

* * *

## Part-2: Content-based Search Engine

(4 marks)

In this part, you need to implement a content-based search engine that uses tf-idf values of all query terms for ranking. You need to calculate tf-idf values for each query term in a page, and rank pages based on the summation of tf-idf values for all query terms. Use "inverted index" you created in Part-1 to locate matching pages for query terms.

Read the following wikipedia page that describes how to calculate tf-idf values:

For this assignment,

* calculate **_term frequency _** tf(t,d) using the raw frequency of a term in a document, i.e. the number of times that term t occurs in document d. See the example in the wikipedia page above.
* calculate **_inverse document frequency_** idf(t, D) by dividing the total number of documents by the number of documents containing the term, and then taking the logarithm of that quotient:   
![][2]  
See the example in the wikipedia page above.

Write a content-based search engine in file `searchTfIdf.c` that given search terms (words) as commandline arguments, outputs (to stdout) top ten pages in descending order of summation of tf-idf values of all search terms (as described above). **Your program must also output the corresponding summation of tf-idf along with each page, separated by a space and using format "%.6f", see example below** (added on May 24).

If number of matches are less than 10, output all of them. Your program must use data available in two files `invertedIndex.txt` and `collection.txt`, and must derive result from them. We will test this program independently to your solutions for Part-1.

Example:

        % searchTfIdf  mars  design
        url25  1.902350
        url31  0.434000

* * *

## Part-3: Hybrid/Meta Search Engine using Rank Aggregation

(3 marks)

In this part, you need to combine search results (ranks) from multiple sources (say from Part-1 and Part-2) using "**Scaled Footrule Rank Aggregation**" method, described below. All the required information for this method are provided below. However, if you are interested, you may want to check out this presentation on [ "Rank aggregation method for the web"][3].

Let T1 and T2 are search results (ranks) obtained from Part-1 and Part-2 respectively. Let's assume S is a union of T1 and T2. A weighted bipartite graph for scaled footrule optimization (C,P,W) is defined as,

* C = set of nodes to be ranked
* P = set of positions available
* **W(c,p)** is the **scaled-footrule distance** (from T1 and T2 ) of a ranking that places element c at position p, given by   
![][4]  
where n is the cardinality (size) of S, |T1| is the cardinality (size) of T1, |T2| is the cardinality (size) of T2,   
T1(x3) is the position of x3 in T1.

For example,

![][5]

The **final ranking** is derived by finding **possible values of position 'P' ** such that the **scaled-footrule distance is minimum**. There are many different ways to assign possible values for 'P'. In the above example P = [1, 3, 2, 5, 4]. Some other possible values are, P = [1, 2, 4, 3, 5], P = [5, 2, 1, 4, 3], P = [1, 2, 3, 4, 5], etc. For n = 5, possible alternatives are 5! For n = 10, possible alternatives would be 10! that is 3,628,800 alternatives. A very simple and obviously inefficient approach could use brute-force search and generate all possible alternatives, calculate scaled-footrule distance for each alternative, and find the alternative with minimum scaled-footrule distance. If you use such a brute-force search, you will receive maximum of 2 marks (out of 3) for Part-3. However, you will be rewarded upto 1 mark if you implement a "smart" algorithm that avoids generating unnecessary alternatives, in the process of finding the minimum scaled-footrule distance. Please document your algorithm such that your tutor can easily understand your logic, and clearly outline how you plan to reduce search space, otherwise you will not be awarded mark for your "smart" algorithm! Yes, it's only a mark, but if you try it, you will find it very challenging and rewarding.

Write a program `scaledFootrule.c` that aggregates ranks from files given as commandline arguments, and output aggregated rank list with minimum scaled footrule distance.

Example, file rankA.txt

        url1
        url3
        url5
        url4
        url2

Example, file rankD.txt

        url3
        url2
        url1
        url4

The following command will read ranks from files "rankA.txt" and "rankD.txt" and outputs minimum scaled footrule distance (using format %.6f) on the first line, followed by the corresponding aggregated rank list.

        % scaledFootrule   rankA.txt  rankD.txt

For the above example, there are **two possible answers, with minimum distance of 1.400000**. Two possible values of P with minnimum distance are:

`   
C = [url1, url2, url3, url4, url5]   
P = [1, 4, 2, 5, 3] and   
P = [1, 5, 2, 4, 3] `

By the way, you need to select any one of the possible values of P that has minium distance, so there could be multiple possible answers. Note that you need to output only one such list.

One possible answer for the above example, for `P = [1, 4, 2, 5, 3] `:

        1.400000
        url1
        url3
        url5
        url2
        url4

Another possible answer for the above example, `P = [1, 5, 2, 4, 3] `:

        1.400000
        url1
        url3
        url5
        url4
        url2

Your program should be able to handle multiple rank files, for example:

        % scaledFootrule   rankA.txt  rankD.txt  newSearchRank.txt  myRank.txt

* * *

## Submission

**Additional files:** You can submit additional supporting files, `*.c` and `*.h`, for this assignment.

IMPORTANT: Make sure that your additional files (*.c) DO NOT have "main" function.

For example, you may implement your graph adt in files `graph.c` and `graph.h` and submit these two files along with other required files as mentioned below. However, make sure that these files do not have "main" function.

I explain below how we will test your submission, hopefully this will answer all of your questions.

You need to submit the following files, along with your supporting files (*.c and *.h):

* pagerank.c
* inverted.c
* searchPagerank.c
* searchTfIdf.c
* scaledFootrule.c
Now say we want to mark your `pagerank.c` program. The auto marking program will take all your supporting files (other *.h and *.c) files, along with `pagerank.c` and execute the following command to generate executable file say called pagerank. Note that the other four files from the above list (`inverted.c`, `searchPagerank.c`, `searchTfIdf.c` and `scaledFootrule.c`) will be removed from the dir:

    % gcc -Wall -lm  *.c  -o pagerank

So we will **not use your Makefile** (if any). The above command will generate object files from your supporting files and the file to be tested (say `pagerank.c`), links these object files and generates executable file, say `pagerank` in the above example. Again, please make sure that you **DO NOT have main function in your supporting files** (other *.c files you submit).

We will use similar approach to generate other four executables (from `inverted.c`, `searchPagerank.c`, `searchTfIdf.c` and `scaledFootrule.c`).

To submit your ass2 files, goto the [** submission page for Ass2 ][6]**
* * *

## Plagiarism

This is an individual assignment. Each student will have to develop their own solution without help from other people. In particular, it is not permitted to exchange code or pseudocode. You are not allowed to use code developed by persons other than yourself. If you have questions about the assignment, ask your tutor Before submitting any work you should read and understand the following very useful guide by the Learning Centre How Not To Plagiarise. All work submitted for assessment must be entirely your own work. We regard unacknowledged copying of material, in whole or part, as an extremely serious offence. For further information, see the Course Information.

* * *

[1]: HowToImplement-Ass2-Part1.pdf
[2]: https://upload.wikimedia.org/math/c/f/d/cfd0e4239f99354e17dae2c117c8a817.png
[3]: http://www.inf.unibz.it/~ricci/SDB/slides/Rank%20aggregation%20methods%20for%20the%20web.pdf
[4]: https://www.cse.unsw.edu.au/~cs1927/16s1/ass/ass02/scaled-footrule-sum.png
[5]: https://www.cse.unsw.edu.au/~cs1927/16s1/ass/ass02/scaled-footrule-example1.png
[6]: https://webcms3.cse.unsw.edu.au/COMP1927/16s1/resources/3147
  