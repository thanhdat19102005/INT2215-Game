# INT2215-Game 
# BATTLE OF CHESS

# Table of Contents
* 0. Thuật ngữ
* 1. Hướng dẫn cài đặt
* 2. Tổng quan
* 3. Ý tưởng
* 4. Vì sao tôi lại chọn game này để thể hiện ?
* 5. Luật chơi
* 6. Các chức năng chính
* 7. Kỹ thuật lập trình
* 8. Hướng đi trong tương lai

# 0. Thuật ngữ
    Checkmate : Chiếu hết
    Capture : Ăn quân
    Check : Chiếu
    En passant : Bắt tốt qua đường
    Rank : Hàng ngang
    File : Hàng dọc

# 1. Hướng dẫn cài đặt
    Tải thư viện SDL(https://github.com/libsdl-org/SDL/releases)
    Tải Visual Studio Code (https://code.visualstudio.com/download) tùy thuộc vào hệ điều hành của máy tính mà bạn đang sử dụng
    Sau đó click vào Code -> Chọn Download ZIP -> Giải nén -> Mở file được nén trong VSCode -> Chạy file main.exe

# 2. Tổng quan
    Trò chơi được lấy cảm hứng từ boardgame nổi tiếng : Cờ vua. 
    Cờ vua, đôi khi còn được gọi là cờ phương Tây hoặc cờ quốc tế để phân biệt với các biến thể như cờ tướng, là một trò chơi board game dành cho hai người. Trò chơi sử dụng một bàn cờ hình vuông chia thành 64 ô vuông nhỏ hơn với 8 hàng ngang và 8 hàng dọc. Mỗi người chơi sẽ bắt đầu với 16 quân cờ, bao gồm 8 con tốt, 2 mã, 2 tượng, 2 xe, 1 hậu và 1 vua. Mục tiêu của người chơi là cố gắng chiếu hết vua đối phương. Vua được gọi bị "chiếu hết" khi đang bị chiếu mà không có cách nào thoát ra. Khi một người chơi bị chiếu hết, trò chơi kết thúc hay nói cách khác người chơi đó đã thua. Cũng có một số trường hợp mà trò chơi có thể kết thúc với tỷ số hòa. Trong suốt ván cờ, hai người chơi thay phiên nhau di chuyển một quân cờ của mình đến một vị trí khác trên bàn cờ. Một người chơi sẽ cầm quân cờ màu trắng và người còn lại sẽ cầm quân cờ đen. Có các quy tắc nhất định về việc di chuyển các quân cờ cũng như việc ăn quân của đối thủ. Người chơi cầm quân cờ trắng sẽ đi trước. Do đó, người chơi cầm quân trắng thường sẽ có một lợi thế nhỏ và có tỷ lệ thắng cao hơn.

# 3. Ý tưởng :
    

References :

* https://www.chessprogramming.org/Main_Page

* https://www.chessprogramming.org/General_Setwise_Operations#Intersection

* https://www.chessprogramming.org/BitScan
* https://www.chessprogramming.org/Dictionary
* https://web.archive.org/web/20071031095918/http://www.brucemo.com/compchess/programming