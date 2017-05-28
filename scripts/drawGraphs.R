#!/usr/local/bin/Rscript
args = commandArgs(trailingOnly=TRUE)

if (length(args)==0) {
  stop("Usage: drawGraphs.R csv-file", call.=FALSE)
} 

samples = 500

csv <- read.csv(file=args[1], head=TRUE, sep=",")
png(paste('plot', format(Sys.time(), "%d_%b_%Y-%H-%M-%S"), '.png', sep=""))

#Read timestamp and cte value from csv file
time = as.numeric(as.character(csv[1:samples+1,1])) 
time = time - time[1]
cte = c(csv[1:samples+1,2])

plot(time, cte, type="l")
lines(c(0,max(time)), c(0,0), col="red")
title("CTE graph")
# Add text using the following Corner_text function:
Corner_text <- function(text, location="topright"){
legend(location,legend=text, bty ="n", pch=NA) }
Corner_text(text=paste("Kp: ", toString(csv[1,2]), "\nKi: ",  toString(csv[1,4]), "\nKd: ",  toString(csv[1,6])))

dev.off()