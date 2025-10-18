#!/bin/bash
echo "=== System Performance Report ===" > performance-report.txt
echo "Generated on: $(date)" >> performance-report.txt
echo "" >> performance-report.txt

echo "=== System Information ===" >> performance-report.txt
uname -a >> performance-report.txt
echo "" >> performance-report.txt

echo "=== CPU Information ===" >> performance-report.txt
lscpu | head -10 >> performance-report.txt
echo "" >> performance-report.txt

echo "=== Memory Usage ===" >> performance-report.txt
free -h >> performance-report.txt
echo "" >> performance-report.txt

echo "=== Disk Usage ===" >> performance-report.txt
df -h >> performance-report.txt
echo "" >> performance-report.txt

echo "=== Load Average ===" >> performance-report.txt
uptime >> performance-report.txt
echo "" >> performance-report.txt

echo "=== Top 10 CPU Processes ===" >> performance-report.txt
ps aux --sort=-%cpu | head -11 >> performance-report.txt
echo "" >> performance-report.txt

echo "=== Top 10 Memory Processes ===" >> performance-report.txt
ps aux --sort=-%mem | head -11 >> performance-report.txt
