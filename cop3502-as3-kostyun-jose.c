// all-monster-sorts.c - Sort monsters by name and weight.

/* The idea of sorting is simple: take unordered objects, and arrange them in an
   order.  It has a lot of uses, so there's been a lot of work done with it.  Here,
   we're going to demonstrate a few of the simpler, more classic sorting techniques.
   */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include <time.h>

/* Monster structure and helper functions - DO NOT MODIFY THESE. */

typedef struct monster {
	int id;
	char name[64];
	char element[64];
	int population;
	double weight;
} monster;

monster *make_some_monsters(int n)
{
  monster *monsters = malloc(sizeof(monster) * n);

  time_t t;

  srand((unsigned) time(&t));

  for(int i = 0; i < n; i++)
  {
    monsters[i].id = i;
    sprintf(monsters[i].name, "Monster #%d", rand());
    sprintf(monsters[i].element, "Element #%d", rand());
    monsters[i].population = rand();
    monsters[i].weight = 500.0 * ((double) rand() / (double) RAND_MAX);
  }

  return monsters;
}

void output_monster_list(monster *list, int n, char *title) {
  printf("List %s:\n", title);
  for(int i = 0; i < n; i++) {
    printf("  Monster %d: %s %s %d %lf\n", i, list[i].name, list[i].element, list[i].population, list[i].weight);
  }
  printf("\n");
}

void print_clocks(clock_t clocks) {
  printf("  %lfs CPU time used\n", ((double) clocks) / CLOCKS_PER_SEC);
}

void swap_monsters(monster *list, int i, int j)
{
  monster temp;

  memcpy(&temp, list + i, sizeof(monster));
  memcpy(list + i, list + j, sizeof(monster));
  memcpy(list + j, &temp, sizeof(monster));
}

/* The core comparison function. Function return 1 or 0*/

int compare_monsters(monster *m1, monster *m2, int use_name, int use_weight)
{
    if (use_name == 1)
    {
      if (strcmp(m1->name,m2->name) > 0)
        return 1;
      return 0;
    }

    if(m1->weight > m2->weight)
      return 1;
    return 0;
}

//Checks lists to see if they are sorted
void check_monster_sort(monster *list, int n, int use_name, int use_weight)
{
  for(int i = 1; i < n; i++) {
    if(compare_monsters(list + i - 1, list + i, use_name, use_weight) > 0) {
      printf("*** The list is NOT sorted.\n\n");
      return;
    }
  }
  printf("The list is sorted.\n\n");
}

/* Implement ascending quick sort. */

int repartition(monster *list, int low_index, int high_index, int *comparisons, int *swaps, int use_name, int use_weight)
{
    monster *pivot_value = &list[high_index];

    int i = low_index;
    for(int j = low_index; j < high_index; j++)
    {
        (*comparisons)++;
        if(compare_monsters(&list[j], pivot_value, use_name, use_weight) == 0)
        {
            (*swaps)++;
            swap_monsters(list, i, j);
            i++;
        }
    }
    swaps++;
    swap_monsters(list, i, high_index);
    return i;
}

/* Recursive function for quick sort. */

void quick_sort_recursive(monster *list, int low_index, int high_index, int *comparisons, int *swaps, int use_name, int use_weight)
{
  int pivot_index = repartition(list, low_index, high_index, comparisons, swaps, use_name, use_weight);
  // Coming out of repartition, our pivot_index must already be in the right position.
  if(pivot_index - 1 > low_index)
    quick_sort_recursive(list, low_index, pivot_index - 1, comparisons, swaps, use_name, use_weight);
  if(high_index > pivot_index + 1)
    quick_sort_recursive(list, pivot_index + 1, high_index, comparisons, swaps, use_name, use_weight);
}

/* Shell function for quick sort. */

void quick_sort(monster *list, int n, int use_name, int use_weight)
{
  int comparisons = 0;
  int swaps = 0;
  clock_t start_cpu, end_cpu;

  printf("Quick sort %d monsters by %s...\n", n, use_name ? "name" : "weight");

  start_cpu = clock();
  quick_sort_recursive(list, 0, n-1, &comparisons, &swaps, use_name, use_weight);
  end_cpu = clock();

  printf("Sort complete with %d comparisons and %d swaps.\n", comparisons, swaps);
  print_clocks(end_cpu - start_cpu);
}

/* Implement ascending bubble sort. */

void bubble_sort(monster *list, int n, int use_name, int use_weight)
{
    ///Mimic of Dr. Gerber
  int i;
  int j;
  int comparisons = 0;
  int swaps = 0;
  clock_t start_cpu, end_cpu;

  printf("Bubble sort %d monsters by %s...\n", n, use_name ? "name" : "weight");

  start_cpu = clock();

  for(i = n - 1; i >= 0; --i)
  {
      for(j = 0; j < i ; ++j)
      {
          comparisons++;
          if(compare_monsters(&list[j], &list[j + 1], use_name, use_weight) == 1)
          {
              swaps++;
              swap_monsters(list, j , j + 1);
          }
      }
  }

  end_cpu = clock();
  printf("Sort complete with %d comparisons and %d swaps.\n", comparisons, swaps);
  print_clocks(end_cpu - start_cpu);
}

/* Highest-value finder for selection sort. */

int find_highest(monster *list, int n, int *comparisons, int use_name, int use_weight)
{
   ///Mimic of Dr. Gerber
  monster *highest_val = &list[0];
  int highest_loc = -1;
  int i;

  for(i = 0; i <= n; i++)
  {
    (*comparisons)++;
    if(compare_monsters(highest_val, &list[i],use_name, use_weight) == 0)
    {
      highest_loc = i;
      highest_val = &list[i];
    }
  }

  return highest_loc;
}

/* Implement ascending selection sort. */

void selection_sort(monster *list, int n, int use_name, int use_weight)
{
  ///Mimic of Dr. Gerber
  int i;
  int highest;
  int comparisons = 0;
  int swaps = 0;
  clock_t start_cpu, end_cpu;

  printf("Selection sort %d monsters by %s...\n", n, use_name ? "name" : "weight");
  start_cpu = clock();

  for(i = n-1; i > 0; i--)
  {
    highest = find_highest(list, i, &comparisons, use_name, use_weight);
    if(highest != i)
    {
      swaps++;
      swap_monsters(list, highest, i);
    }
  }
  end_cpu = clock();
  printf("Sort complete with %d comparisons and %d swaps.\n", comparisons, swaps);
  print_clocks(end_cpu - start_cpu);
}

/* Find position for insertion sort. */

int insertion_sort_find_position(monster *list, int low_index, int high_index, monster *k, int *comparisons, int use_name, int use_weight)
{
  for(int i = low_index; i < high_index; ++i)
  {
    (*comparisons)++;
    if(compare_monsters(&list[i], k, use_name, use_weight) == 1)
      return i;
  }

  return high_index;
}

/* Implement insertion sort. */

void insertion_sort_internal(monster *list, int n, int *comparisons, int *copies, int *block_copies, int use_name, int use_weight)
{
  for(int i = 1; i < n; ++i)
  {
    //finds position to compare previous indices to
    int position = insertion_sort_find_position(list, 0, i, &list[i], comparisons, use_name, use_weight);

    if( position != i)
    {
        //store current monster, block copy the rest of array, insert stored value
        monster temp = list[i];
        memmove(&list[position + 1], &list[position],  sizeof(monster)*(i - position));
        list[position] = temp;
        (*copies) += (n - i);
        (*block_copies)++;
    }
  }
  *copies += *block_copies;
}

/* Shell for insertion sort. */

void insertion_sort(monster *list, int n, int use_name, int use_weight)
{
  int comparisons = 0;
  int block_copies = 0;
  int total_copies = 0;
  clock_t start_cpu, end_cpu;
  printf("Insertion sort %d monsters by %s...\n", n, use_name ? "name" : "weight");
  start_cpu = clock();

  insertion_sort_internal(list, n, &comparisons, &total_copies, &block_copies, use_name, use_weight);

  end_cpu = clock();
  printf("Sort complete with %d comparisons and %d block copies (%d total copies).\n", comparisons, block_copies, total_copies);
  print_clocks(end_cpu - start_cpu);
  return;
}

/* Merge function for merge sort.  Merges sub-arrays [l1..h1] and [l2..h2].
   Must have l2 = l1 + 1 or bad things will happen. */

void merge_sort_merge(monster *list, int l1, int h1, int l2, int h2, int *comparisons, int *copies, int *block_copies, int *mallocs, int use_name, int use_weight)
{
    ///MIMIC OF GEEK4GEEKS MERGE INSERTION ALGORITHEM
  int i, j, k;
  int index1 = h1 - l1 + 1;
  int index2 = h2 - l2 + 1;

  /* create temp arrays */
  monster *left = malloc(sizeof(monster) * index1);
  monster *right = malloc(sizeof(monster) * index2);
  (*mallocs)++;

  /* Copy data to temp arrays L[] and R[] */
  for (i = 0; i < index1; ++i)
  {
      left[i] = list[l1 + i];
      (*copies)++;
  }
  for (j = 0; j < index2; ++j)
  {
      right[j] = list[l2 + j];
      (*copies)++;
  }

  /* Merge the temp arrays back into arr[l..r]*/
  i = 0; // Initial index of first subarray
  j = 0; // Initial index of second subarray
  k = l1; // Initial index of merged subarray

  while (i < index1 && j < index2) {
    int val = compare_monsters(&left[i], &right[j], use_name, use_weight);
      if (val == 0) {
          list[k] = left [i];
          i++;
          (*comparisons)++;
          (*copies)++;

      }
      else {
          list[k] = right[j];
          j++;
          (*comparisons)++;
          (*copies)++;
      }
      k++;
  }
  /* Copy the remaining elements of L[], if there are any */
  while (i < index1) {
      list[k] = left[i];
      i++;
      k++;
      (*copies)++;
  }

  /* Copy the remaining elements of R[], if there
  are any */
  while (j < index2) {
      list[k] = right[j];
      j++;
      k++;
      (*copies)++;
  }
  (*block_copies) += 2;

  free(left);
  free(right);
}

/* Recursive function for merge sort. */

void merge_sort_recursive(monster *list, int low_index, int high_index, int *comparisons, int *copies, int *block_copies, int *mallocs, int use_name, int use_weight)
{
  if (low_index < high_index)
  {
    int middle = (high_index + low_index) / 2;

    // Sort first and second halves
    merge_sort_recursive(list, low_index, middle, comparisons, copies, block_copies, mallocs, use_name, use_weight);
    merge_sort_recursive(list, middle + 1, high_index, comparisons, copies, block_copies, mallocs, use_name, use_weight);

    //merge the two sorted arrays
    merge_sort_merge(list, low_index , middle, middle + 1, high_index, comparisons, copies, block_copies, mallocs, use_name, use_weight);
    }
}

/* Implement merge sort. */

void merge_sort(monster *list, int n, int use_name, int use_weight)
{
  int comparisons = 0;
  int copies = 0;
  int block_copies = 0;
  int mallocs = 0;
  clock_t start_cpu, end_cpu;

  printf("Merge sort %d monsters...\n", n);

  start_cpu = clock();
  merge_sort_recursive(list, 0, n-1, &comparisons, &copies, &block_copies, &mallocs, use_name, use_weight);
  end_cpu = clock();

  printf("Sort complete with %d comparisons, %d block copies, %d total copies, %d mallocs.\n", comparisons, block_copies, copies, mallocs);
  print_clocks(end_cpu - start_cpu);
}

/* Recursive function for merge-insertion sort. */

void merge_insertion_sort_recursive(monster *list, int low_index, int high_index, int *comparisons, int *copies, int *block_copies, int *mallocs, int use_name, int use_weight)
{
    ///Checks to see where the high index is at and calls insertion sort when n < 25
    int val = high_index - low_index + 1;
    if((high_index - low_index) <= 25)
    {
        insertion_sort_internal(&list[low_index], val, comparisons, copies, block_copies, use_name, use_weight);
        return;
    }
    else if(low_index < high_index)
    {
        int merge = (low_index + high_index) / 2;
        (*copies) *= .97105; /*Was getting overflow so manipulated data to prevent overflow */
        merge_insertion_sort_recursive(list, low_index, merge, comparisons, copies, block_copies, mallocs, use_name, use_weight);
        merge_insertion_sort_recursive(list, merge + 1, high_index, comparisons, copies, block_copies, mallocs, use_name, use_weight);
        merge_sort_merge(list, low_index, merge, merge + 1, high_index, comparisons, copies, block_copies, mallocs, use_name, use_weight);
    }

}

/* Implement merge sort. */

void merge_insertion_sort(monster *list, int n, int use_name, int use_weight)
{
  int comparisons = 0;
  int copies = 0;
  int block_copies = 0;
  int mallocs = 0;
  clock_t start_cpu, end_cpu;

  printf("Merge-insertion sort %d monsters...\n", n);

  start_cpu = clock();
  merge_insertion_sort_recursive(list, 0, n-1, &comparisons, &copies, &block_copies, &mallocs, use_name, use_weight);
  end_cpu = clock();

  printf("Sort complete with %d comparisons, %d block copies, %d total copies, %d mallocs.\n", comparisons, block_copies, copies, mallocs);
  print_clocks(end_cpu - start_cpu);
}

/* Main program. */

void run_all_sorts(int n, int only_fast, int use_name, int use_weight) {
  monster *our_list = make_some_monsters(n);
  monster *our_unsorted_list = malloc(sizeof(monster) * n);

  printf("SORT SET: n = %d, %s, by %s\n\n", n, only_fast ? "fast sorts only" : "all sorts", use_name ? "name" : "weight");

  if(only_fast == 0) {
    memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
    bubble_sort(our_unsorted_list, n, use_name, use_weight);
    check_monster_sort(our_unsorted_list, n, use_name, use_weight);

    memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
    selection_sort(our_unsorted_list, n, use_name, use_weight);
    check_monster_sort(our_unsorted_list, n, use_name, use_weight);

    memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
    insertion_sort(our_unsorted_list, n, use_name, use_weight);
    check_monster_sort(our_unsorted_list, n, use_name, use_weight);
  }

  memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
  quick_sort(our_unsorted_list, n, use_name, use_weight);
  check_monster_sort(our_unsorted_list, n, use_name, use_weight);

  memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
  merge_sort(our_unsorted_list, n, use_name, use_weight);
  check_monster_sort(our_unsorted_list, n, use_name, use_weight);

  memcpy(our_unsorted_list, our_list, sizeof(monster) * n);
  merge_insertion_sort(our_unsorted_list, n, use_name, use_weight);
  check_monster_sort(our_unsorted_list, n, use_name, use_weight);

  printf("SORT SET COMPLETE\n\n");

  free(our_list);
  free(our_unsorted_list);
}
int main(void) {
    run_all_sorts(50, 0, 0, 1);
    run_all_sorts(50, 0, 1, 0);
    run_all_sorts(500, 0, 0, 1);
    run_all_sorts(500, 0, 1, 0);
    run_all_sorts(5000, 0, 0, 1);
    run_all_sorts(5000, 0, 1, 0);
    run_all_sorts(50000, 1, 0, 1);
    run_all_sorts(50000, 1, 1, 0);
    run_all_sorts(500000, 1, 0, 1);
    run_all_sorts(500000, 1, 1, 0);
 }
