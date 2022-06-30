/** Function to shift the current view to the left/right
 *
 * @param: "arg->i" stores the number of tags to shift right (positive value)
 *          or left (negative value)
 */
void
shiftview(const Arg *arg)
{
	Arg shifted;
	shifted.ui = selmon->tagset[selmon->seltags];
	if (arg->i > 0) /* left circular shift */
		shifted.ui = (shifted.ui << arg->i) | (shifted.ui >> (LENGTH(tags) - arg->i));
	else /* right circular shift */
		shifted.ui = (shifted.ui >> (-arg->i) | shifted.ui << (LENGTH(tags) + arg->i));
	view(&shifted);
}
