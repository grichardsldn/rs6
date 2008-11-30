tempo 12
section verse
channel hihats
midi 3
{
	hdisco.pat,
	hdisco.pat,
	hdisco.pat,
	hdisco.pat,
	hdisco.pat,
	hdisco.pat,
	hdisco.pat,
	hdisco.pat,
}
channel drums
midi 1
{
	d1234.pat,
	d1234.pat,
	d1234.pat,
	d123e4.pat,
	d1234.pat,
	d1234.pat,
	d1234.pat,
	ds24na.pat,
}
channel bass
midi 2
{
octoff.pat,
octoff.pat+2,
octoff.pat-2,
octoff.pat-4,
}
end

section intro
channel drums
midi 1
{
test.pat,
test.pat ,
test.pat ,
}
channel bass
midi 2
{
test2.pat,
test2.pat,
}
end

play
#intro
verse
verse
end

