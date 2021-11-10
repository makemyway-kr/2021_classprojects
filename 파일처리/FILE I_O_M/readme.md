<h1>Project No.1 of File Processing class of 2021</h1><br>
<h2>making program for file input & output and also put utillity for modifying and merging them</h2>
<br>
<br>
<br>
developed on Ubuntu 18.04 (WSL 2) & GCC 7.5.0 with C<br>
<br>
<h3>list of utilities of this program</h3><br>
<ol>
  <li>copy</li><br>
  copied txt file with fread & fwrite functions<br>
  <li>read</li><br>
  read the intended part of txt file with fseek function<br>
  <li>merge</li>
  merge the txt file with another txt file by opening file with "a"mode and write after eof<br>
  <li>overwrite</li>
  overwriting the file with fseek function and write after the offset on the written file<br>
  <li>insert</li>
  inserting another data between already written words by storing after-coming words in temp memory<br>
  and then write data I want to insert&temp memory<br>
  <li>delete</li>
  store all data except the one that I want to delete. And then write the whole file again <br>
  Well...it's not smart but there was limitation that I have to use only C library<br>
</ol>
