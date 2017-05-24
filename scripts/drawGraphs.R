#!/usr/local/bin/Rscript
args = commandArgs(trailingOnly=TRUE)

if (length(args)==0) {
  stop("Input csv file must be supplied", call.=FALSE)
} 

csv <- read.csv(file=args[1], head=TRUE,sep=",")
png('plot.png')
samples = 500
cte = head(csv[,2],samples)
time = head(csv[,1],samples)
plot(time, cte, type="l")
lines(c(0,max(time)), c(0,0), col="red")

dev.off()