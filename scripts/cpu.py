import os
import psutil
import time
import shutil

def get_cpu_temperature():
    temp = os.popen("vcgencmd measure_temp").readline()
    temp = temp.replace("temp=", "").replace("'C\n", "")
    return float(temp)

def get_cpu_usage():
    return psutil.cpu_percent(percpu=True)

def get_terminal_width():
    return shutil.get_terminal_size().columns

def display_cpu_usage_bar(cpu_usage, bar_width):
    for i, usage in enumerate(cpu_usage):
        filled_width = int(usage * bar_width / 100)
        bar = '█' * filled_width + ' ' * (bar_width - filled_width)
        print(f"Core {i}: {usage:5.1f}% |{bar}|")

while True:
    cpu_temp = get_cpu_temperature()
    cpu_usage = get_cpu_usage()
    terminal_width = get_terminal_width()
    bar_width = terminal_width - 20  # Adjust the value based on the desired spacing

    print(f"CPU Temperature: {cpu_temp:.1f}°C")
    display_cpu_usage_bar(cpu_usage, bar_width)
    print("------------------------")

    time.sleep(1)