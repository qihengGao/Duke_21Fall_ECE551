#include <iostream>

#include "page.hpp"
#include "story.hpp"

int main(int argc, char ** argv) {
  /* Usage check. */
  if (argc != 2) {
    std::cerr << "Usage: ./cyoa-step2 directory_path" << std::endl;
    exit(EXIT_FAILURE);
  }
  /* Instantiate story object, check then start the advencture. */
  std::string directory(argv[1]);
  Story story(directory);
  /* Check choices page are in range, all pages are referenced, and the story has both WIN and LOSE page. */
  story.check();
  /* Start adventure if all requirement match. */
  story.start();
  return EXIT_SUCCESS;
}
