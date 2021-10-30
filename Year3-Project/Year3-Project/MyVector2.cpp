#include "MyVector2.h"

/// <summary>
/// get length of vector using sqrt of the sum of the squares
/// </summary>
/// <param name="t_vector">input vector</param>
/// <returns>length</returns>
float vectorLength(sf::Vector2f t_vector)
{
	float sumOfSquares = (t_vector.x * t_vector.x ) + (t_vector.y * t_vector.y);
	const float length = std::sqrt(sumOfSquares);
	return length;
}

float vectorLengthSquared(sf::Vector2f t_vector)
{
	float sumOfSquares = (t_vector.x * t_vector.x) + (t_vector.y * t_vector.y);
	return sumOfSquares;
}

float vectorCorssProduct(sf::Vector2f t_vectorA, sf::Vector2f t_VectorB)
// Vx * Uy - Vy * Ux

{
	float vectorCorssProduct=(t_vectorA.x * t_VectorB.y) - (t_vectorA.y * t_VectorB.x);
	return vectorCorssProduct;
}

float vectorDotProduct(sf::Vector2f t_vectorA, sf::Vector2f t_VectorB)
  // Vx * Ux + Vy * Uy
{
	float vectorDotProduct = (t_vectorA.x * t_VectorB.x) + (t_vectorA.y * t_VectorB.y);
	return vectorDotProduct;
}

float vectorAngleBetween(sf::Vector2f t_vectorA, sf::Vector2f t_VectorB)
//result always 0 >= && <= 180
{
	float vectorProduct{ 0.0f };
	vectorProduct = vectorDotProduct(t_vectorA,t_VectorB);
	float lengthA{ 0.0f };
	float lengthB{ 0.0f };
	float theta{ 0.0f };
	float vectorAngleBetween{ 0.0f };
	const float PI{ 3.14159265359f };
	float deg{ 0.0f };
	lengthA = vectorLength(t_vectorA);
	lengthB = vectorLength(t_VectorB);
	theta = (vectorProduct) / (lengthA * lengthB);
	vectorAngleBetween = acos(theta);
	vectorAngleBetween = (vectorAngleBetween) * (180 / PI);
	if (vectorAngleBetween >= 180 && vectorAngleBetween <= 360)
	{
		vectorAngleBetween = vectorAngleBetween - 180;
	}



	return vectorAngleBetween;
}

sf::Vector2f vectorRotateBy(sf::Vector2f t_vector, float t_angleRadians)
{
	float xCoord{ 0.0f };
	float yCoord{ 0.0f };
	xCoord = cos(t_angleRadians) * (t_vector.x) - sin(t_angleRadians) * (t_vector.y);
	yCoord = sin(t_angleRadians) * (t_vector.x) + cos(t_angleRadians) * (t_vector.y);
	return sf::Vector2f{ xCoord,yCoord };
} 


sf::Vector2f vectorProjection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	float vectorProduct{ 0.0f };
	vectorProduct = vectorDotProduct(t_vector, t_onto);
	float length{ 0.0f };
	length = vectorLength(t_onto);
	float projectNum{ 0.0f };
	projectNum = vectorProduct / (length * length);
	float xCoord{ 0.0f };
	float yCoord{ 0.0f };
	xCoord = projectNum * (t_onto.x);
	yCoord = projectNum * (t_onto.y);

	return sf::Vector2f{ xCoord,yCoord };
}

sf::Vector2f vectorRejection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	//the vector minus its vector projection 
	float vectorProduct{ 0.0f };
	vectorProduct = vectorDotProduct(t_vector, t_onto);
	float length{ 0.0f };
	length = vectorLength(t_onto);
	float projectNum{ 0.0f };
	projectNum = vectorProduct / (length * length);
	float xProjection{ 0.0f };
	float yProjection{ 0.0f };
	xProjection = projectNum * (t_onto.x);
	yProjection = projectNum * (t_onto.y);
	float xCoord{ 0.0f };
	float yCoord{ 0.0f };
	xCoord = t_vector.x - xProjection;
	yCoord = t_vector.y - yProjection;

	return sf::Vector2f{ xCoord,yCoord };
}

float vectorScalarProjection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	float vectorProduct{ 0.0f };
	vectorProduct = vectorDotProduct(t_vector, t_onto);
	float lengthA{ 0.0f };
	float lengthB{ 0.0f };
	float theta{ 0.0f };
	float vectorAngleBetween{ 0.0f };
	const float PI{ 3.14159265359f };
	float deg{ 0.0f };
	lengthA = vectorLength(t_vector);
	lengthB = vectorLength(t_onto);
	theta = (vectorProduct) / (lengthA * lengthB);
	vectorAngleBetween = acos(theta);
	vectorAngleBetween = (vectorAngleBetween) * (180 / PI);
	float scalarProjection{ 0.0f };
	scalarProjection = vectorProduct / lengthB;
	if (vectorAngleBetween >=0 && vectorAngleBetween <= 90)
	{
		scalarProjection = scalarProjection;
	}
	else if (vectorAngleBetween > 90 && vectorAngleBetween <= 180)
	{
		scalarProjection = (-scalarProjection);

	}
	return scalarProjection;
}

sf::Vector2f vectorUnitVector(sf::Vector2f t_vector)
    // length of ans is one
{
    // vector divided by lenght
	float lenght{ 0.0f };
	float xCoord{ 0.0f };
	float yCoord{ 0.0f };
	lenght = vectorLength(t_vector);
	xCoord = t_vector.x / lenght;
	yCoord = t_vector.y / lenght;
	
	return sf::Vector2f{ xCoord,yCoord };
}
