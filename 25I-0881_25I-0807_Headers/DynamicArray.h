#pragma once

template <class T>
class DynamicArray
{
private:
	T* data;
	int size;
	int capacity;

	void resize(int newCapacity)
	{
		if (newCapacity < 1)
		{
			newCapacity = 1;
		}

		T* newData = new T[newCapacity];
		for (int i = 0; i < size; i += 1)
		{
			newData[i] = data[i];
		}

		delete[] data;
		data = newData;
		capacity = newCapacity;
	}

public:
	DynamicArray()
	{
		size = 0;
		capacity = 4;
		data = new T[capacity];
	}

	~DynamicArray()
	{
		delete[] data;
	}

	void pushBack(const T& value)
	{
		if (size >= capacity)
		{
			resize(capacity * 2);
		}

		data[size] = value;
		size += 1;
	}

	void removeAt(int index)
	{
		if (index < 0 || index >= size)
		{
			return;
		}

		for (int i = index; i < size - 1; i += 1)
		{
			data[i] = data[i + 1];
		}

		size -= 1;
	}

	T& get(int index)
	{
		return data[index];
	}

	const T& get(int index) const
	{
		return data[index];
	}

	void set(int index, const T& value)
	{
		if (index < 0 || index >= size)
		{
			return;
		}

		data[index] = value;
	}

	void clear()
	{
		size = 0;
	}

	int getSize() const
	{
		return size;
	}
};
