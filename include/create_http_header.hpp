void create_statusline(std::string& resonse)
{

}

void create_response_header(std::string& resonse)
{

}
void create_representation_header(std::string& resonse)
{

}

std::string create_response()
{
	std::string response;

	create_statusline(response);
	create_header(response);
	create_data(response);
	return response;
}