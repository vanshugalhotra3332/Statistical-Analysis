#include <stdio.h>
#include <conio.h>
#include <math.h>

float ind_mean(float series[], int size) // sigma X / n
{
    float sum = 0, mean;
    for (int i = 0; i < size; i++)
    {
        sum += series[i];
    }
    mean = sum / size;
    return mean;
}

float disc_mean(float series[][2], int size) // sigma Fx/ sigma F
{
    float sum = 0, mean, sigmaf = 0;
    for (int i = 0; i < size; i++)
    {
        sum += series[i][0] * series[i][1];
        sigmaf += series[i][1];
    }
    mean = sum / sigmaf;
    return mean;
}

float con_mean(float series[][3], int size) // sigma fm/sigma f..
{
    float mean, disc_ser_con[size][2];
    for (int i = 0; i < size; i++)
    {
        disc_ser_con[i][0] = (series[i][0] + series[i][1]) / 2; // m = l+u/2
        disc_ser_con[i][1] = series[i][2];                      // f
    }

    mean = disc_mean(disc_ser_con, size);
    return mean;
}

float ind_quartile(float series[], int size, float value) // median = q2, so we can calculate q1,q2,q3 using just 1 function
{
    float temp, median, med_index, decimal_part; // ignore the name median this function is for median , q1, q3
    int int_part;

    // logic to convert the series into ascending order
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (series[i] > series[j])
            {
                temp = series[i];
                series[i] = series[j];
                series[j] = temp;
            }
        }
    }

    med_index = (size + 1) * value;
    int_part = (int)med_index;
    decimal_part = med_index - int_part;

    median = series[int_part - 1] + (decimal_part * (series[int_part] - series[int_part - 1]));
    return median;
}

float disc_quartile(float series[][2], int size, float value)
{
    float temp, median, temp_freq, cf_class;
    int cf[size], cf_ind;
    float sigmaf = 0;

    // logic to convert the series into ascending order
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (series[i][0] > series[j][0])
            {
                temp = series[i][0];
                temp_freq = series[i][1];

                series[i][0] = series[j][0];
                series[i][1] = series[j][1];

                series[j][0] = temp;
                series[j][1] = temp_freq;
            }
        }
    }
    for (int i = 0; i < size; i++)
    {
        sigmaf += series[i][1];
        cf[i] = sigmaf;
    }

    cf_class = (sigmaf + 1) * value;

    for (int i = 0; i < size; i++)
    {
        if (cf_class <= cf[i])
        {
            cf_ind = i;
            break;
        }
    }
    median = series[cf_ind][0];
    return median;
}

float con_quartile(float series[][3], int size, float value)
{
    float temp, median, temp_freq, cf_class, tempU;
    int cf[size], cf_ind;
    float sigmaf = 0, L, f, cff, lu;

    // logic to convert the series into ascending order
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (series[i][0] > series[j][0])
            {
                temp = series[i][0];
                temp_freq = series[i][2];
                tempU = series[i][1];

                series[i][0] = series[j][0];
                series[i][1] = series[j][1];
                series[i][2] = series[j][2];

                series[j][0] = temp;
                series[j][2] = temp_freq;
                series[j][1] = tempU;
            }
        }
    }
    for (int i = 0; i < size; i++)
    {
        sigmaf += series[i][2];
        cf[i] = sigmaf;
    }
    cf_class = sigmaf * value;

    for (int i = 0; i < size; i++)
    {
        if (cf[i] >= cf_class)
        {
            L = series[i][0];
            cff = (i >= 1) ? cf[i - 1] : 0;
            f = series[i][2];
            lu = series[i][1] - L;
            break;
        }
    }

    median = L + ((cf_class - cff) / f) * lu;
    return median;
}

float ind_mode(float series[], int size) // calculating mode for individual series
{
    float mode, series_freq[size][2];
    float freq = 1, max_freq, check_double, mean, median;
    int mode_index;
    for (int i = 0; i < size; i++) // looping to convert the individual series to discrete one to get highest frequency
    {
        for (int j = i + 1; j < size; j++)
        {
            if (series[i] == series[j])
            {
                freq++;
            }
        }
        series_freq[i][0] = series[i];
        series_freq[i][1] = freq;
        freq = 1;
    }
    max_freq = series_freq[0][1];
    for (int i = 0; i < size; i++)
    {
        if (series_freq[i][1] > max_freq)
            max_freq = series_freq[i][1];
    }

    check_double = 0;
    for (int i = 0; i < size; i++)
    {
        if (series_freq[i][1] == max_freq)
        {
            mode_index = i;
            check_double++;
        }
    }
    if (check_double <= 1) // if there is only one highest frequency
    {
        mode = series_freq[mode_index][0];
    }

    else
    { // mode = 3 median - 2 mean
        mean = ind_mean(series, size);
        median = ind_quartile(series, size, 0.5);
        mode = (3 * median) - (2 * mean);
    }
    return mode;
}

float disc_mode(float series[][2], int size) // to find mode for discrete series
{
    float max_freq, check_double, mode, sigmaf;
    int mode_index, g1_ind[size], gi2, gi3, gi4, gi5, gi6;
    float g1[size], g2[size], g3[size], g4[size], g5[size], g6[size];
    float maxg1, maxg2, maxg3, maxg4, maxg5, maxg6;

    max_freq = series[0][1];
    for (int i = 0; i < size; i++)
    {
        if (series[i][1] > max_freq)
            max_freq = series[i][1];
    }

    check_double = 0;
    for (int i = 0; i < size; i++)
    {
        if (series[i][1] == max_freq)
        {
            mode_index = i;
            check_double++;
        }
    }
    if (check_double <= 1) // if there is only one highest frequency its simple
    {
        mode = series[mode_index][0];
    }

    else
    { // grouping method:  to break the tie between 2 or more values of same frequency
        // filling the groups
        int j = 0, x = 0;
        sigmaf = 0;
        for (int i = 0; i < size; i++, j += 2, x += 3)
        {
            g1[i] = series[i][1];                                           // only frequencies
            g2[i] = series[j][1] + series[j + 1][1];                        // only 1+2
            g3[i] = series[j + 1][1] + series[j + 2][1];                    // only 2+3
            g4[i] = series[x][1] + series[x + 1][1] + series[x + 2][1];     // only 1+2+3
            g5[i] = series[x + 1][1] + series[x + 2][1] + series[x + 3][1]; // only 2+3+4
            g6[i] = series[x + 2][1] + series[x + 3][1] + series[x + 4][1]; // only 3+4+5
            sigmaf += series[i][1];
        }
        maxg1 = g1[0];
        maxg2 = g2[0];
        maxg3 = g3[0];
        maxg4 = g4[0];
        maxg5 = g5[0];
        maxg6 = g6[0];

        // --------------------------------Looping in each group to find the maximum value----------------------------
        for (int i = 0; i < size; i++)
        {
            if (g1[i] > maxg1)
            {
                maxg1 = g1[i];
            }
        }
        int v2 = 0;
        while (g2[v2] < sigmaf && g2[v2] > 0)
        {
            if (g2[v2] > maxg2)
            {
                maxg2 = g2[v2];
            }
            v2++;
        }
        int v3 = 0;
        while (g3[v3] < sigmaf && g2[v3] > 0)
        {
            if (g3[v3] > maxg3)
            {
                maxg3 = g3[v3];
            }
            v3++;
        }
        int v4 = 0;
        while (g4[v4] < sigmaf && g4[v4] > 0)
        {
            if (g4[v4] > maxg4)
            {
                maxg4 = g4[v4];
            }
            v4++;
        }
        int v5 = 0;
        while (g5[v5] < sigmaf && g5[v5] > 0)
        {
            if (g5[v5] > maxg5)
            {
                maxg5 = g5[v5];
            }
            v5++;
        }
        int v6 = 0;
        while (g6[v6] < sigmaf && g6[v6] > 0)
        {
            if (g6[v6] > maxg6)
            {
                maxg6 = g6[v6];
            }
            v6++;
        }
        // ------------------------------------looping in each group to find the index at which the maximum value exists
        int ind = 0;
        for (int i = 0; i < size; i++)
        {
            if (series[i][1] == maxg1)
            {
                g1_ind[ind] = i;
                ind++;
            }
        }

        int vi2 = 0;
        while (g2[vi2] < sigmaf && g2[vi2] > 0)
        {
            if (g2[vi2] == maxg2)
            {
                gi2 = vi2;
            }
            vi2++;
        }
        int vi3 = 0;
        while (g3[vi3] < sigmaf && g3[vi3] > 0)
        {
            if (g3[vi3] == maxg3)
            {
                gi3 = vi3;
            }
            vi3++;
        }
        int vi4 = 0;
        while (g4[vi4] < sigmaf && g4[vi4] > 0)
        {
            if (g4[vi4] == maxg4)
            {
                gi4 = vi4;
            }
            vi4++;
        }
        int vi5 = 0;
        while (g5[vi5] < sigmaf && g5[vi5] > 0)
        {
            if (g5[vi5] == maxg5)
            {
                gi5 = vi5;
            }
            vi5++;
        }
        int vi6 = 0;
        while (g6[vi6] < sigmaf && g6[vi6] > 0)
        {
            if (g6[vi6] == maxg6)
            {
                gi6 = vi6;
            }
            vi6++;
        }

        // -----------------------Adjusting the frequencies according to the groups to break the tie--------
        int vb = 0;
        while (g1_ind[vb] < sigmaf && g1_ind[vb] > 0)
        {
            series[g1_ind[vb]][1] += 1;
            vb++;
        }
        // for g2
        series[((gi2 + 1) * 2) - 1 - 1][1] += 1;
        series[((gi2 + 1) * 2) - 1][1] += 1;

        // for g3
        series[((gi3 + 1) * 2) - 1][1] += 1;
        series[((gi3 + 1) * 2) + 1 - 1][1] += 1;

        // for g4
        series[((gi4 + 1) * 3) - 1][1] += 1;
        series[((gi4 + 1) * 3) - 1 - 1][1] += 1;
        series[((gi4 + 1) * 3) - 2 - 1][1] += 1;

        // for g5
        series[((gi5 + 1) * 3) + 1 - 1][1] += 1;
        series[((gi5 + 1) * 3) - 1][1] += 1;
        series[((gi5 + 1) * 3) - 1 - 1][1] += 1;

        // for g6
        series[((gi6 + 1) * 3) + 2 - 1][1] += 1;
        series[((gi6 + 1) * 3) + 1 - 1][1] += 1;
        series[((gi6 + 1) * 3) - 1][1] += 1;

        max_freq = series[0][1];
        for (int i = 0; i < size; i++)
        {
            if (series[i][1] > max_freq)
                max_freq = series[i][1];
        }

        for (int i = 0; i < size; i++)
        {
            if (series[i][1] == max_freq)
            {
                mode_index = i;
            }
        }

        mode = series[mode_index][0];
    }
    return mode;
}

float con_mode(float series[][3], int size)
{
    return 0;
}

float ind_hm_mean(float series[], int size) // harmonic mean
{
    float sum = 0, hmean;
    for (int i = 0; i < size; i++)
    {
        sum += 1 / series[i];
    }
    hmean = size / sum;
    return hmean;
}

float disc_hm_mean(float series[][2], int size)
{
    float sum = 0, hmean, sigmaf = 0;
    for (int i = 0; i < size; i++)
    {
        sum += series[i][1] / series[i][0];
        sigmaf += series[i][1];
    }
    hmean = sigmaf / sum;
    return hmean;
}

float con_hm_mean(float series[][3], int size)
{
    float hmean, disc_ser_con[size][2];
    for (int i = 0; i < size; i++)
    {
        disc_ser_con[i][0] = (series[i][0] + series[i][1]) / 2; // m = l+u/2
        disc_ser_con[i][1] = series[i][2];                      // f
    }

    hmean = disc_hm_mean(disc_ser_con, size);
    return hmean;
}

float ind_gm_mean(float series[], int size) // geometric mean = sigma LOG x / N
{
    float gmean;
    double sum = 0;

    for (int i = 0; i < size; i++)
    {
        sum += log10(series[i]);
    }
    gmean = pow(10, (sum / size));
    return gmean;
}

float disc_gm_mean(float series[][2], int size)
{
    float gmean, sigmaf = 0;
    double sum = 0;

    for (int i = 0; i < size; i++)
    {
        sum += series[i][1] * log10(series[i][0]); // f * log x
        sigmaf += series[i][1];
    }
    gmean = pow(10, (sum / sigmaf));
    return gmean;
}

float con_gm_mean(float series[][3], int size)
{
    float gmean, disc_ser_con[size][2];
    for (int i = 0; i < size; i++)
    {
        disc_ser_con[i][0] = (series[i][0] + series[i][1]) / 2; // m = l+u/2
        disc_ser_con[i][1] = series[i][2];                      // f
    }

    gmean = disc_gm_mean(disc_ser_con, size);
    return gmean;
}

float *ind_range(float series[], int size) // this function return array of highest and lowest values
{
    static float range[2];
    float temp;
    // logic to convert the series into ascending order
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (series[i] > series[j])
            {
                temp = series[i];
                series[i] = series[j];
                series[j] = temp;
            }
        }
    }

    range[0] = series[size - 1];
    range[1] = series[0];
    return range;
}

float *disc_range(float series[][2], int size)
{
    static float range[2];
    float temp, temp_freq;
    // logic to convert the series into ascending order
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (series[i][0] > series[j][0])
            {
                temp = series[i][0];
                temp_freq = series[i][1];

                series[i][0] = series[j][0];
                series[i][1] = series[j][1];

                series[j][0] = temp;
                series[j][1] = temp_freq;
            }
        }
    }
    range[0] = series[size - 1][0];
    range[1] = series[0][0];
    return range;
}

float *con_range(float series[][3], int size) // answer may differ if series is inclusive
{
    static float range[2];
    float temp, temp_freq, tempU;
    // logic to convert the series into ascending order
    for (int i = 0; i < size; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (series[i][0] > series[j][0])
            {
                temp = series[i][0];
                temp_freq = series[i][2];
                tempU = series[i][1];

                series[i][0] = series[j][0];
                series[i][1] = series[j][1];
                series[i][2] = series[j][2];

                series[j][0] = temp;
                series[j][2] = temp_freq;
                series[j][1] = tempU;
            }
        }
    }
    range[0] = series[size - 1][1];
    range[1] = series[0][0];
    return range;
}

float *ind_mean_deviation(float series[], int size)
{
    static float mean_deviation[6];
    float mean, median, mode, mean_ser[size], median_ser[size], mode_ser[size], sigmaf_mean, sigmaf_med, sigmaf_mode;

    mean = ind_mean(series, size);
    median = ind_quartile(series, size, 0.5);
    mode = ind_mode(series, size);

    for (int i = 0; i < size; i++)
    {
        mean_ser[i] = fabs(series[i] - mean);
        median_ser[i] = fabs(series[i] - median);
        mode_ser[i] = fabs(series[i] - mode);
    }
    sigmaf_mean = sigmaf_med = sigmaf_mode = 0;
    for (int i = 0; i < size; i++)
    {
        sigmaf_mean += mean_ser[i];
        sigmaf_med += median_ser[i];
        sigmaf_mode += mode_ser[i];
    }
    mean_deviation[0] = sigmaf_mean / size;
    mean_deviation[1] = sigmaf_med / size;
    mean_deviation[2] = sigmaf_mode / size;

    mean_deviation[3] = mean_deviation[0] / mean;   // coefficient
    mean_deviation[4] = mean_deviation[1] / median; // coefficient
    mean_deviation[5] = mean_deviation[2] / mode;   // coefficient
    return mean_deviation;
}

float *disc_mean_deviation(float series[][2], int size)
{
    static float mean_deviation[6];
    float mean, median, mode, mean_ser[size][2], median_ser[size][2], mode_ser[size][2];
    float sigmaf, sigmaf_mean, sigmaf_med, sigmaf_mode;

    mean = disc_mean(series, size);
    median = disc_quartile(series, size, 0.5);
    mode = disc_mode(series, size);

    for (int i = 0; i < size; i++)
    {
        mean_ser[i][0] = fabs(series[i][0] - mean);
        median_ser[i][0] = fabs(series[i][0] - median);
        mode_ser[i][0] = fabs(series[i][0] - mode);
    }
    sigmaf_mean = sigmaf_med = sigmaf_mode = sigmaf = 0;
    for (int i = 0; i < size; i++)
    {
        sigmaf_mean += mean_ser[i][0] * series[i][1];
        sigmaf_med += median_ser[i][0] * series[i][1];
        sigmaf_mode += mode_ser[i][0] * series[i][1];
        sigmaf += series[i][1];
    }
    mean_deviation[0] = sigmaf_mean / sigmaf;
    mean_deviation[1] = sigmaf_med / sigmaf;
    mean_deviation[2] = sigmaf_mode / sigmaf;

    mean_deviation[3] = mean_deviation[0] / mean;   // coefficient
    mean_deviation[4] = mean_deviation[1] / median; // coefficient
    mean_deviation[5] = mean_deviation[2] / mode;   // coefficient
    return mean_deviation;
}

float *con_mean_deviation(float series[][3], int size)
{
    static float mean_deviation[6];
    float mean, median, mean_ser[size][3], median_ser[size][3];
    float sigmaf, sigmaf_mean, sigmaf_med, cofmean, cofmed;

    mean = con_mean(series, size);
    median = con_quartile(series, size, 0.5);

    for (int i = 0; i < size; i++)
    {
        mean_ser[i][0] = fabs((series[i][1] + series[i][0]) / 2 - mean);
        median_ser[i][0] = fabs((series[i][1] + series[i][0]) / 2 - median);
    }
    sigmaf_mean = sigmaf_med = sigmaf = 0;
    for (int i = 0; i < size; i++)
    {
        sigmaf_mean += mean_ser[i][0] * series[i][2];
        sigmaf_med += median_ser[i][0] * series[i][2];
        sigmaf += series[i][2];
    }
    mean_deviation[0] = sigmaf_mean / sigmaf;
    mean_deviation[1] = sigmaf_med / sigmaf;
    mean_deviation[2] = 0; // cant calculate for mode rn

    mean_deviation[3] = mean_deviation[0] / mean;   // coefficient
    mean_deviation[4] = mean_deviation[1] / median; // coefficient
    mean_deviation[5] = 0;                          // coefficient

    return mean_deviation;
}

float *ind_standard_deviation(float series[], int size)
{
    static float standard_deviation[2];
    float sd, cofsd, xsquare[size], var, sigmax, sigmaxx, mean;

    mean = ind_mean(series, size);

    for (int i = 0; i < size; i++)
    {
        xsquare[i] = series[i] * series[i]; // x square
    }
    sigmax = sigmaxx = 0;
    for (int i = 0; i < size; i++)
    {
        sigmax += series[i];
        sigmaxx += xsquare[i];
    }

    var = (sigmaxx / size) - ((sigmax / size) * (sigmax / size));
    standard_deviation[0] = sqrt(var);
    standard_deviation[1] = standard_deviation[0] / mean;
    return standard_deviation;
}

float *disc_standard_deviation(float series[][2], int size)
{
    static float standard_deviation[2];
    float sd, cofsd, fxsquare[size], var, sigmaf, sigmaxx, sigmafx, mean;

    mean = disc_mean(series, size);

    for (int i = 0; i < size; i++)
    {
        fxsquare[i] = (series[i][0] * series[i][0]) * series[i][1]; // f * x square
    }
    sigmaf = sigmaxx = sigmafx = 0;
    for (int i = 0; i < size; i++)
    {
        sigmaf += series[i][1];
        sigmaxx += fxsquare[i];
        sigmafx += series[i][0] * series[i][1];
    }

    var = (sigmaxx / sigmaf) - ((sigmafx / sigmaf) * (sigmafx / sigmaf));
    standard_deviation[0] = sqrt(var);
    standard_deviation[1] = standard_deviation[0] / mean;
    return standard_deviation;
}

float *con_standard_deviation(float series[][3], int size)
{
    static float standard_deviation[2];
    float sd, cofsd, fxsquare[size], var, sigmaf, sigmaxx, sigmafx, mean;

    mean = con_mean(series, size);

    for (int i = 0; i < size; i++)
    {
        fxsquare[i] = (((series[i][0] + series[i][1]) / 2) * ((series[i][0] + series[i][1]) / 2)) * series[i][2]; // f*m2
    }
    sigmaf = sigmaxx = sigmafx = 0;
    for (int i = 0; i < size; i++)
    {
        sigmaf += series[i][2];
        sigmaxx += fxsquare[i];
        sigmafx += ((series[i][0] + series[i][1]) / 2) * series[i][2];
    }

    var = (sigmaxx / sigmaf) - ((sigmafx / sigmaf) * (sigmafx / sigmaf));
    standard_deviation[0] = sqrt(var);
    standard_deviation[1] = standard_deviation[0] / mean;
    return standard_deviation;
}

int main()
{
    int ser, elm, runagain, ops;
    float mean, median, mode, harmonic_mean, geometric_mean, q1, q3, rang, qrang, cofrang, qq1, qq3;
    float qrange, cofqrange;
    float *range, *mean_deviation, *standard_deviation, *variance;
    printf("------Basic Statistical Operations-----------------\n");
    do
    {
        printf("Which Type of Series?\n1.) Individual\n2.) Discrete\n3.) Continuous Series\n");
        scanf("%d", &ser);

        printf("How many elements?\n");
        scanf("%d", &elm);

        float ind_ser[elm];
        float disc_ser[elm][2];
        float con_ser[elm][3];

        // In first switch we are accepting the data from user and storing it in a array
        switch (ser)
        {
        case 1:
            printf("Enter the Elements of the Series: (x)\n");
            for (int i = 0; i < elm; i++)
            {
                scanf("%f", &ind_ser[i]);
            }
            break;

        case 2:
            printf("First Enter The Element and Then Its Frequency:(x-f) \n");
            for (int i = 0; i < elm; i++)
            {
                printf("X: (%d)\n", i + 1);
                scanf("%f", &disc_ser[i][0]); // for X
                printf("F: (%d)\n", i + 1);
                scanf("%f", &disc_ser[i][1]); // for F
            }
            break;

        case 3:
            printf("Lower Limit - Upper Limit - Frequency\n");
            for (int i = 0; i < elm; i++)
            {
                printf("Lower Limit: (%d)\n", i + 1);
                scanf("%f", &con_ser[i][0]); // for L
                printf("Upper Limit: (%d)\n", i + 1);
                scanf("%f", &con_ser[i][1]); // for U
                printf("Frequency: (%d)\n", i + 1);
                scanf("%f", &con_ser[i][2]); // for F
            }
            break;

        default:
            printf("Invalid Option For Type Series :(\n");
            break;
        }

        if (ser == 1 || ser == 2 || ser == 3)
        {
            printf("How many operations you wanna perform. Please Specify:\n");
            scanf("%d", &ops);
            int operations[ops];
            printf("\n1.) Arithmetic Mean\n2.) Median\n3.) Mode\n4.) Harmonic Mean\n5.) Geometric Mean\n");
            printf("6.) Quartile 1(Q1)\n7.) Quartile 3(Q3)\n8.) Range\n9.) Inter Quartile Range\n");
            printf("10.) Mean Deviation\n11.) Standard Deviation\n12.) Variance\n");
            for (int i = 0; i < ops; i++)
            {
                scanf("%d", &operations[i]);
            }
            printf("Calculating.......\n");
            // In this switch we are performing statistical operations on the data
            for (int i = 0; i < ops; i++)
            {
                switch (operations[i])
                {
                case 1:
                    if (ser == 1)
                    {
                        mean = ind_mean(ind_ser, elm);
                        printf("The mean is %f\n", mean);
                    }

                    else if (ser == 2)
                    {
                        mean = disc_mean(disc_ser, elm);
                        printf("The mean is %f\n", mean);
                    }

                    else if (ser == 3)
                    {
                        mean = con_mean(con_ser, elm);
                        printf("The mean is %f\n", mean);
                    }

                    break;

                case 2:
                    if (ser == 1)
                    {
                        median = ind_quartile(ind_ser, elm, 0.5);
                        printf("The median is %f\n", median);
                    }

                    else if (ser == 2)
                    {
                        median = disc_quartile(disc_ser, elm, 0.5);
                        printf("The median is %f\n", median);
                    }

                    else if (ser == 3)
                    {
                        median = con_quartile(con_ser, elm, 0.5);
                        printf("The Median is %f\n", median);
                    }
                    break;

                case 3:
                    if (ser == 1)
                    {
                        mode = ind_mode(ind_ser, elm);
                        printf("The Mode is %f\n", mode);
                    }

                    else if (ser == 2)
                    {
                        mode = disc_mode(disc_ser, elm);
                        printf("The Mode is %f\n", mode);
                    }

                    else if (ser == 3)
                    {
                        mode = con_mode(con_ser, elm);
                        printf("Sorry Mode for continuous series ain't working!");
                        printf("The Mode is %f\n", mode);
                    }
                    break;

                case 4:
                    if (ser == 1)
                    {
                        harmonic_mean = ind_hm_mean(ind_ser, elm);
                        printf("The Harmonic Mean is %f\n", harmonic_mean);
                    }

                    else if (ser == 2)
                    {
                        harmonic_mean = disc_hm_mean(disc_ser, elm);
                        printf("The Harmonic Mean is %f\n", harmonic_mean);
                    }

                    else if (ser == 3)
                    {
                        harmonic_mean = con_hm_mean(con_ser, elm);
                        printf("The Harmonic Mean is %f\n", harmonic_mean);
                    }
                    break;

                case 5:
                    if (ser == 1)
                    {
                        geometric_mean = ind_gm_mean(ind_ser, elm);
                        printf("The Geometric Mean is %f\n", geometric_mean);
                    }

                    else if (ser == 2)
                    {
                        geometric_mean = disc_gm_mean(disc_ser, elm);
                        printf("The Geometric Mean is %f\n", geometric_mean);
                    }

                    else if (ser == 3)
                    {
                        geometric_mean = con_gm_mean(con_ser, elm);
                        printf("The Geometric Mean is %f\n", geometric_mean);
                    }
                    break;

                case 6:
                    if (ser == 1)
                    {
                        q1 = ind_quartile(ind_ser, elm, 0.25);
                        printf("The Quartile 1(Q1) is %f\n", q1);
                    }

                    else if (ser == 2)
                    {
                        q1 = disc_quartile(disc_ser, elm, 0.25);
                        printf("The Quartile 1(Q1) is %f\n", q1);
                    }

                    else if (ser == 3)
                    {
                        q1 = con_quartile(con_ser, elm, 0.25);
                        printf("The Quartile 1(Q1) is %f\n", q1);
                    }
                    break;

                case 7:
                    if (ser == 1)
                    {
                        q3 = ind_quartile(ind_ser, elm, 0.75);
                        printf("The Quartile 3(Q3) is %f\n", q3);
                    }

                    else if (ser == 2)
                    {
                        q3 = disc_quartile(disc_ser, elm, 0.75);
                        printf("The Quartile 3(Q3) is %f\n", q3);
                    }

                    else if (ser == 3)
                    {
                        q3 = con_quartile(con_ser, elm, 0.75);
                        printf("The Quartile 3(Q3) is %f\n", q3);
                    }
                    break;

                case 8:
                    if (ser == 1)
                    {
                        range = ind_range(ind_ser, elm);
                        rang = range[0] - range[1];
                        cofrang = rang / (range[0] + range[1]);

                        printf("The Range is %f\n", rang);
                        printf("The Coefficient of Range is %f\n", cofrang);
                    }

                    else if (ser == 2)
                    {
                        range = disc_range(disc_ser, elm);
                        rang = range[0] - range[1];
                        cofrang = rang / (range[0] + range[1]);

                        printf("The Range is %f\n", rang);
                        printf("The Coefficient of Range is %f\n", cofrang);
                    }

                    else if (ser == 3)
                    {
                        range = con_range(con_ser, elm);
                        rang = range[0] - range[1];
                        cofrang = rang / (range[0] + range[1]);

                        printf("The Range is %f\n", rang);
                        printf("The Coefficient of Range is %f\n", cofrang);
                    }
                    break;

                case 9:
                    if (ser == 1)
                    {
                        qq1 = ind_quartile(ind_ser, elm, 0.25);
                        qq3 = ind_quartile(ind_ser, elm, 0.75);
                        qrange = qq3 - qq1;
                        cofqrange = qrange / (qq3 + qq1);

                        printf("The Inter Quartile Range is %f\n", qrange);
                        printf("The Quartile Deviation / Semi Inter Quartile Range is %f\n", qrange / 2);
                        printf("The Coefficient of Quartile Deviation is %f\n", cofqrange);
                    }

                    else if (ser == 2)
                    {
                        qq1 = disc_quartile(disc_ser, elm, 0.25);
                        qq3 = disc_quartile(disc_ser, elm, 0.75);
                        qrange = qq3 - qq1;
                        cofqrange = qrange / (qq3 + qq1);

                        printf("The Inter Quartile Range is %f\n", qrange);
                        printf("The Quartile Deviation / Semi Inter Quartile Range is %f\n", qrange / 2);
                        printf("The Coefficient of Quartile Deviation is %f\n", cofqrange);
                    }

                    else if (ser == 3)
                    {
                        qq1 = con_quartile(con_ser, elm, 0.25);
                        qq3 = con_quartile(con_ser, elm, 0.75);
                        qrange = qq3 - qq1;
                        cofqrange = qrange / (qq3 + qq1);

                        printf("The Inter Quartile Range is %f\n", qrange);
                        printf("The \tQuartile Deviation     OR    Semi Inter Quartile Range is %f\n", qrange / 2);
                        printf("The Coefficient of Quartile Deviation is %f\n", cofqrange);
                    }
                    break;

                case 10:
                    if (ser == 1)
                    {
                        mean_deviation = ind_mean_deviation(ind_ser, elm);

                        printf("The Mean Deviation From Mean is %f\n", mean_deviation[0]);
                        printf("The Coefficient of Mean Deviation From Mean is %f\n", mean_deviation[3]);

                        printf("The Mean Deviation From Median is %f\n", mean_deviation[1]);
                        printf("The Coefficient of Mean Deviation From Median is %f\n", mean_deviation[4]);

                        printf("The Mean Deviation From Mode is %f\n", mean_deviation[2]);
                        printf("The Coefficient of Mean Deviation From Mode is %f\n", mean_deviation[5]);
                    }

                    else if (ser == 2)
                    {
                        mean_deviation = disc_mean_deviation(disc_ser, elm);

                        printf("The Mean Deviation From Mean is %f\n", mean_deviation[0]);
                        printf("The Coefficient of Mean Deviation From Mean is %f\n", mean_deviation[3]);

                        printf("The Mean Deviation From Median is %f\n", mean_deviation[1]);
                        printf("The Coefficient of Mean Deviation From Median is %f\n", mean_deviation[4]);

                        printf("The Mean Deviation From Mode is %f\n", mean_deviation[2]);
                        printf("The Coefficient of Mean Deviation From Mode is %f\n", mean_deviation[5]);
                    }

                    else if (ser == 3)
                    {
                        mean_deviation = con_mean_deviation(con_ser, elm);

                        printf("The Mean Deviation From Mean is %f\n", mean_deviation[0]);
                        printf("The Coefficient of Mean Deviation From Mean is %f\n", mean_deviation[3]);

                        printf("The Mean Deviation From Median is %f\n", mean_deviation[1]);
                        printf("The Coefficient of Mean Deviation From Median is %f\n", mean_deviation[4]);

                        printf("Sorry Mean Deviation From Mode Can't be calculated right now!\n");
                    }
                    break;

                case 11:
                    if (ser == 1)
                    {
                        standard_deviation = ind_standard_deviation(ind_ser, elm);
                        printf("The Standard Deviation is: %f\n", standard_deviation[0]);
                        printf("The Coefficient of Standard Deviation is: %f\n", standard_deviation[1]);
                    }
                    else if (ser == 2)
                    {
                        standard_deviation = disc_standard_deviation(disc_ser, elm);
                        printf("The Standard Deviation is: %f\n", standard_deviation[0]);
                        printf("The Coefficient of Standard Deviation is: %f\n", standard_deviation[1]);
                    }
                    else if (ser == 3)
                    {
                        standard_deviation = con_standard_deviation(con_ser, elm);
                        printf("The Standard Deviation is: %f\n", standard_deviation[0]);
                        printf("The Coefficient of Standard Deviation is: %f\n", standard_deviation[1]);
                    }
                    break;

                case 12:
                    if (ser == 1)
                    {
                        standard_deviation = ind_standard_deviation(ind_ser, elm);
                        printf("The Variance is: %f\n", standard_deviation[0] * standard_deviation[0]);
                        printf("The Coefficient of SVarianceis: %f\n", standard_deviation[1]*100);
                    }
                    else if (ser == 2)
                    {
                        standard_deviation = disc_standard_deviation(disc_ser, elm);
                        printf("The Variance is: %f\n", standard_deviation[0] * standard_deviation[0]);
                        printf("The Coefficient of Variance is: %f\n", standard_deviation[1]*100);
                    }
                    else if (ser == 3)
                    {
                        standard_deviation = con_standard_deviation(con_ser, elm);
                        printf("The Variance is: %f\n", standard_deviation[0] * standard_deviation[0]);
                        printf("The Coefficient of Variance is: %f\n", standard_deviation[1]*100);
                    }
                    break;

                default:
                    printf("Invalid Option for Type Operation :( \n");
                    break;
                }
            }
        }
        do
        {
            printf("Wanna run again?: yes/no (1/0):\n");
            scanf("%d", &runagain);
            if (runagain == 1 || runagain == 0)
            {
                if (runagain == 0)
                {
                    printf("Goodbye! :)\n");
                }
            }
            else
            {
                printf("Invalid option Input only (1/0): \n");
                runagain = 2;
            }
        } while (runagain == 2);

    } while (runagain != 0);

    getch();
    return 0;
}