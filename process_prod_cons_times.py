import statistics

n_list  = [1,10,100,1000]
np_list = [1,1,1,1,2,4,8]
nc_list = [1,2,4,8,1,1,1]

# 95% confidence interval
def compute_mean_and_interval(list):
    for i in range(len(list)):
        list[i] = float(list[i][:-1].replace(",","."))
    return statistics.mean(list), statistics.stdev(list)*1.96/(len(list)**0.5)

def main():
    f = open("prod_cons_result_2023-0507_16:40:17")
    lines = f.readlines()
    for n_index in range(4):
        starting_line = (1 + 11 * 7) * n_index + 2
        for np_nc_combination_index in range(7):
            print("n=",n_list[n_index]," np=", np_list[np_nc_combination_index]," nc=", nc_list[np_nc_combination_index])
            #print(starting_line)
            values = lines[starting_line:starting_line+10]
            mean, dev = compute_mean_and_interval(values)
            print(round(mean,4), " +- ", round(dev,4))
            starting_line += 11

if __name__ == "__main__":
    main()