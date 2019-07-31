#include <rpc_server.h>
#include "Hall.hpp"
using namespace std;
using namespace rest_rpc;
using namespace rpc_service;
#include <fstream>


//struct dummy{
//	int add(connection* conn, int a, int b) { return a + b; }
//};
//
//std::string translate(connection* conn, const std::string& orignal) {
//	std::string temp = orignal;
//	for (auto& c : temp) { 
//		c = std::toupper(c); 
//	}
//	return temp;
//}
//
//void hello(connection* conn, const std::string& str) {
//	std::cout << "hello " << str << std::endl;
//}
//
//struct person {
//	int id;
//	std::string name;
//	int age;
//
//	MSGPACK_DEFINE(id, name, age);
//};
//
//std::string get_person_name(connection* conn, const person& p) {
//	return p.name;
//}
//
//person get_person(connection* conn) {
//	return { 1, "tom", 20 };
//}
//
//void upload(connection* conn, const std::string& filename, const std::string& content) {
//	std::cout << content.size() << std::endl;
//	std::ofstream file(filename, std::ios::binary);
//	file.write(content.data(), content.size());
//}
//
//std::string download(connection* conn, const std::string& filename) {
//	std::ifstream file(filename, std::ios::binary);
//	if (!file) {
//		return "";
//	}
//
//	file.seekg(0, std::ios::end);
//	size_t file_len = file.tellg();
//	file.seekg(0, std::ios::beg);
//	std::string content;
//	content.resize(file_len);
//	file.read(&content[0], file_len);
//	std::cout << file_len << std::endl;
//
//	return content;
//}
//
//qps g_qps;
//
//std::string get_name(connection* conn, const person& p) {
//	g_qps.increase();
//	return p.name;
//}
//

Hall GameHall;

uint32_t RpcRegister(connection* conn, string name, string passwd)
{
    //cout << "Get a Register request name: " << name << " passwd " << passwd << endl;
    //return 12345;

    return GameHall.Register(name, passwd);
}

uint32_t RpcLogin(connection* conn, uint32_t id, string passwd)
{
    return GameHall.Login(id, passwd);
}

bool RpcMatchAndWait(connection* conn, uint32_t id)
{
    return GameHall.PushIdInMatchPool(id); // 阻塞式的
}

int RpcPlayerReady(connection* conn, uint32_t id)
{
    return GameHall.IsPlayerReady(id);
}

string RpcBoard(connection* conn, uint32_t room_id)
{
    return GameHall.GetPlayerBoard(room_id);
}

uint32_t RpcPlayerRoomId(connection* conn, uint32_t id)
{
    return GameHall.GetPlayerRoomId(id);
}

char RpcPlayerPiece(connection* conn, uint32_t room_id, uint32_t id)
{
    return GameHall.GetPlayerPiece(room_id, id);
}

bool RpcIsMyTurn(connection* conn, uint32_t room_id, uint32_t id)
{
    return GameHall.IsMyTurn(room_id, id);
}

void RpcStep(connection* conn, uint32_t room_id, uint32_t id, int x, int y)
{
    return GameHall.Step(room_id, id, x, y);
}

char RpcJudge(connection* conn, uint32_t room_id, uint32_t id)
{
    return GameHall.Judge(room_id, id);
}

bool RpcPopMatchPool(connection* conn, uint32_t id)
{
    return GameHall.PopIdMatchPool(id);
}

int main()
{
	rpc_server server(8989, 4);
    LOG(INFO, "线程初始化...等待中");

	server.register_handler("RpcRegister", RpcRegister);
	server.register_handler("RpcLogin", RpcLogin);
	server.register_handler("RpcMatchAndWait", RpcMatchAndWait);
	server.register_handler("RpcPlayerReady", RpcPlayerReady);
	server.register_handler("RpcPlayerRoomId", RpcPlayerRoomId);
	server.register_handler("RpcPlayerPiece", RpcPlayerPiece);
	server.register_handler("RpcBoard", RpcBoard);
	server.register_handler("RpcIsMyTurn", RpcIsMyTurn);
	server.register_handler("RpcStep", RpcStep);
	server.register_handler("RpcJudge", RpcJudge);
	server.register_handler("RpcPopMatchPool", RpcPopMatchPool);


    LOG(INFO, "接口全部完善...等待中");
    LOG(INFO, "服务启动...等待中");
    GameHall.InitHall();

//	dummy d;
//	server.register_handler("add", &dummy::add, &d);
//	server.register_handler("translate", translate);
//	server.register_handler("hello", hello);
//	server.register_handler("get_person_name", get_person_name);
//	server.register_handler("get_person", get_person);
//	server.register_handler("upload", upload);
//	server.register_handler("download", download);
//	server.register_handler("get_name", get_name);

	server.run();

	std::string str;
	std::cin >> str;
}
