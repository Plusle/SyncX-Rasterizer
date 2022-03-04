#include <math/vector.hpp>
#include <math/matrix.hpp>
#include <math/linear.hpp>
#include <math/transfrom.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

using namespace SyncX;

int main(int argc, char** argv) {
	//auto coso = static_cast<float>(std::cos(0.f));
	//auto sino = static_cast<float>(std::sin(0.f));
	//std::cout << coso << ' ' << sino << std::endl;

	//Transform mvp;
	//mvp.SetModelMatrix(Vector3f(0, 1, 0), 0, Vector3f(0, 0, 0));
	//mvp.SetViewMatrix(Vector3f(0, 0, 3), Vector3f(0, 0, -1), Vector3f(0, 1, 0));
	//mvp.SetPrespectiveMatrix(0.1, 100, 4.0 / 3.0, 45);
	//auto mat = mvp.projection * mvp.view * mvp.model;
	//std::cout << mvp.model << '\n' << mvp.view << '\n' << mvp.projection << std::endl;
	//std::cout << mat << std::endl;

	//constexpr float x = 27.9;
	//std::vector<Vector3f> vs{ {-x, 0, x}, {x, 0, x}, {-x, 0, -x}, {x, 0, -x} };
	//std::vector<Vector4f> dst;
	//auto trans = [&mat](const auto& v) -> Vector4f {
	//	return mat * Vector4f(v.x, v.y, v.z, 1.0);
	//};
	//std::transform(vs.cbegin(), vs.cend(), std::back_inserter(dst), trans);
	//for (const auto& v : dst) {
	//	std::cout << v;
	//}

	Vector3f v[3];
	v[0] = Vector3f(0.f, 1.f, 1.0f);
	v[1] = Vector3f(std::sqrt(3) / 2.0f, -0.5f, 1.0f);
	v[2] = Vector3f(-std::sqrt(3) / 2.0f, -0.5f, 1.0f);
	auto [a, b, c] = ComputeBarycentricCoordinate(Vector3f(0, 0, 1), v);

	std::cout << a << " " << b << " " << c << std::endl;

	auto v2 = BarycentricIntepolation<Vector2f>(a, b, c, Vector2f(0.5, 1), Vector2f(0, 0), Vector2f(1, 0), 1);

	std::cout << v2 << std::endl;
	return 0;
}