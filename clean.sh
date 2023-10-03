#!/bin/sh

rm **/**/output/*.jpg
cd parallel
ls . | for i in `cat`; 
do 
       cd $i; 
       echo $i;
       make clean;
       cd ..;
done
cd ..

cd serial
ls . | for i in `cat`; 
do 
       cd $i;
       echo $i
       make clean;
       cd ..;
done

cd ..