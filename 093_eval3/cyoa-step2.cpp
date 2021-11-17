#include <iostream>

#include "page.hpp"
#include "story.hpp"
int mian(int argc, char ** args) {
  /* Usage check. */
  if (argc != 2) {
    std::cerr << "Usage: ./cyoa-step2 directory_path" << std::endl;
    exit(EXIT_FAILURE);
  }
  /* Instantiate story object, check then start the advencture. */
  std::string directory(args[1]);
  Story story(directory);
  /* Check choices page are in range, all pages are referenced, and the story has both WIN and LOSE page. */
  story.check();
  story.start();
  return EXIT_SUCCESS;
}
