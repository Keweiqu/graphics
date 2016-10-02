The general logic of this program is such that the draw function will render a linked-list of shapes. Shapes will be drawed polymorphically by checking each shape's type and call the corresponding draw shape function such as draw_triangle.

To insert a new shape into the linked-list. There are three steps, exec_mouse_down, exec_cursor(only if mouse is pressed), and exec_mouse_up. In exec_mouse_down, a new point is created in the spot it is clicked, and check with previous element in the linked-list, to see if this is the right time to draw the requested object. (e.g. it requires two points draw a circle) So if the linked-list currently does not contain two new points(i.e. points that are not hidden), then it's not the time to draw a circle, but instead keep waiting for the user to click and create more points.

In exec_cursor event, only when mouse is still pressed, it will update the point's coord based on the mouse's position. Or insert a point occasionally under specific condition.

In exec_mouse_up event, it will do some clear up stuff such as set dashed to FALSE. Indicate a drawing is finished, finalize color option, and set the option of fill/nofill.

Line is specificed by two end points; triangle by three vertices; rect by two diagonal vertices; circle by a diameter; and polygon by all vertices.

The program is checked under valgrind and shows no memo leaks.
