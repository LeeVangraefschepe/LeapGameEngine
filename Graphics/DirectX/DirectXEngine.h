class DirectXEngine
{
public:
	DirectXEngine() = default;
	~DirectXEngine() = default;
	DirectXEngine(const DirectXEngine& other) = delete;
	DirectXEngine(DirectXEngine&& other) = delete;
	DirectXEngine& operator=(const DirectXEngine& other) = delete;
	DirectXEngine& operator=(DirectXEngine&& other) = delete;

	void Initialize();
	void Draw();

private:
};