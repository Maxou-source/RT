
bool	equal(float a, float b)
{
	if (abs(a - b) < EPSILON)
		return (true);
	return (false);
}