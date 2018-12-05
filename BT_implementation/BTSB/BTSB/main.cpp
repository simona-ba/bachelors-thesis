#include <iostream>
#include <GLW/Window.h>



class MyObserver : public glw::WindowEventObserver
{
  // this method is called every time someone clicks on window
  // see implementation of WindowEventObserver for many other methods
  virtual void OnMouseDown(int button, int mods) override
  {
    std::cout << "Mouse key was pressed" << std::endl;
  }
};



int main()
{
  // specify initial window parameters
  glw::Window::InitParameters params;
  params.aa_samples = 0;
  params.try_share_contexts = false;
  params.resolution = glm::uvec2(640, 480);

  // create new window with parameters
  glw::Window window(params);

  // implementation of user interaction
  MyObserver input_control;

  // register input_control to receive events from window
  window.ObserverRegister(&input_control);

  // repeat until close button on window is pressed
  while (!window.ShouldClose())
  {
    // DRAWING HAPPENS HERE

    // after drawing swap buffers
    window.PollEvents();

    // inform all window observers about actions
    window.SwapBuffers();
  }

  return 0;
}