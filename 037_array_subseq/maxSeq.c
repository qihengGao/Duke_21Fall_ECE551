#include <stdio.h>
#include <stdlib.h>

size_t maxSeq(int * arr, size_t n) {
  if (arr == NULL || n == 0)
    return 0;
  // dp[j]: LIS from [0, .., j]
  // dp[j] = 1 + dp[j - 1] if arr[j] > arr[j-1]
  // else stay unchanged
  size_t maxLen = 0;
  size_t cached[n];
  // get every LIS starting from each idx
  for (size_t i = 0; i < n; i++) {
    size_t j = i + 1;
    while (j < n && arr[j] > arr[j - 1])
      j++;
    cached[i] = (j - i);
  }
  // find the maximum
  for (size_t i = 0; i < n; i++) {
    if (cached[i] > maxLen)
      maxLen = cached[i];
  }
  return maxLen;
}
