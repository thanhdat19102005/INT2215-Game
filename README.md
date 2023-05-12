# BATTLE OF CHESS

<em style = "font-size: 20px">INT2215 - Game</em>

Đây là dự án đầu tiên của mình về lập trình Game bằng ngôn ngữ C/C++ kết hợp với thư viện đồ họa SDL2.

<p style = "font-family: Arial; font-size: 30px; font-weight: bold; color: yellow"> K67 VNU-UET </p>

# Thông tin tác giả 

🎓 Mình là Nguyễn Tuấn Ngọc, sinh viên năm nhất (tại thời điểm làm game) của lớp K67-J thuộc Đại học Công nghệ-ĐHQGHN.

<p> 📮Email: ngoctuannguyen1980123@gmail.com </p>

## 🔎 Table of Contents
*  [Thuật ngữ](#glossary)
*  [Hướng dẫn cài đặt](#setting)
*  [Luật chơi](#rule_of_game)
*  [Ý tưởng](#idea)
*  [Vì sao mình lại chọn game này để thể hiện ?](#why)
*  [Các chức năng chính](#primary_funtion)
*  [Kỹ thuật lập trình](#programming_technique)
*  [Hướng đi trong tương lai](#future_development)

## 📖 Thuật ngữ<a name = "glossary"></a>
- Checkmate : Chiếu hết
- Capture : Ăn quân
- Check : Chiếu
- En passant : Bắt tốt qua đường
- Rank : Hàng ngang
- File : Hàng dọc

## 🛠️ Hướng dẫn cài đặt<a name = "setting"></a>
-   Tải thư viện SDL
    - SDL : https://github.com/libsdl-org/SDL/releases 
    - SDL_mixer : https://github.com/libsdl-org/SDL_mixer (âm thanh)
    - SDL_ttf : https://github.com/libsdl-org/SDL_ttf/releases (font chữ)
    - SDL_image : https://github.com/libsdl-org/SDL_image (hình ảnh)

- Tải Visual Studio Code (https://code.visualstudio.com/download) tùy thuộc vào hệ điều hành của máy tính mà bạn đang sử dụng

- Sau đó click vào Code -> Chọn Download ZIP -> Giải nén -> Mở file được nén trong VSCode -> Chạy file main.exe

## ⚖️ Luật chơi<a name = "rule_of_game"></a>
Trò chơi được lấy cảm hứng từ boardgame nổi tiếng : Cờ vua. 
Cờ vua, đôi khi còn được gọi là cờ phương Tây hoặc cờ quốc tế để phân biệt với các biến thể như cờ tướng, là một trò chơi board game dành cho hai người. Trò chơi sử dụng một bàn cờ hình vuông chia thành 64 ô vuông nhỏ hơn với 8 hàng ngang và 8 hàng dọc. Mỗi người chơi sẽ bắt đầu với 16 quân cờ, bao gồm 8 con tốt, 2 mã, 2 tượng, 2 xe, 1 hậu và 1 vua. Mục tiêu của người chơi là cố gắng chiếu hết vua đối phương. Vua được gọi bị "chiếu hết" khi đang bị chiếu mà không có cách nào thoát ra. Khi một người chơi bị chiếu hết, trò chơi kết thúc hay nói cách khác người chơi đó đã thua. Cũng có một số trường hợp mà trò chơi có thể kết thúc với tỷ số hòa. Trong suốt ván cờ, hai người chơi thay phiên nhau di chuyển một quân cờ của mình đến một vị trí khác trên bàn cờ. Một người chơi sẽ cầm quân cờ màu trắng và người còn lại sẽ cầm quân cờ đen. Có các quy tắc nhất định về việc di chuyển các quân cờ cũng như việc ăn quân của đối thủ. Người chơi cầm quân cờ trắng sẽ đi trước. Do đó, người chơi cầm quân trắng thường sẽ có một lợi thế nhỏ và có tỷ lệ thắng cao hơn.

## 💡 Ý tưởng : <a name = "idea"></a>

- Game của mình được xây dựng dựa trên các ý tưởng sau :
    + Xây dựng bàn cờ dựa trên Bitboard(một số nguyên biểu diễn dưới dạng 64-bit đại diện cho việc có hay không có của quân cờ)

    + Các phép toán Bitwise để thực hiện việc thay đổi trạng thái của quân cờ 

    + Thư viện đồ họa SDL2 nhằm hiển thị giao diện

    + Negamax search với alpha-beta pruning

    + Quiescence search

    + Principal variation search

    + Iterated deepening

    + Aspiration window

    + Null move reductions

    + Zobrist hashing

    + Transposition tables
    
    + FEN string

## ❓ Vì sao mình lại chọn game này để thể hiện <a name = "why"></a>

- Do bản thân mình có đam mê với các tựa game giải đố, rèn luyện não bộ, đặc biệt là Cờ vua - bộ môn mình khá yêu thích và có mong muốn tái hiện lại trò chơi này.
- Mình có hứng thú và đang rèn luyện trên con đường để trở thành một Master AI.
- Giúp mình rèn luyện được tư duy thuật toán.
- Tiếp cận được một phần nhỏ trong Reinforcement Learning và xây dựng được Bot chơi cờ vua.
- Giúp mình có khả năng tìm kiếm thông tin, học những cái mới, xây dựng được kĩ năng lập trình, đặc biệt là OOP.
- Rèn luyện kĩ năng thiết kế (chia nhỏ, xây dựng module), lập trình, cải tiến và duy trì.

## 🐧 Các chức năng chính <a name = "primary_function"></a>

![](https://github.com/ngoctuannguyen/INT2215-Game/blob/main/res/Screenshot1.png)

- Để bắt đầu trò chơi, ta click nút Start, bên trắng sẽ đi trước.

- Ta có thể chọn chế độ 2 người chơi (Human vs Human) hoặc người chơi với máy (Human vs Computer). Với computer thì có thể lựa chọn level tùy ý : 1 là dễ nhất, 9 là khó nhất.

- Nút ![](https://github.com/ngoctuannguyen/INT2215-Game/blob/main/res/Screenshot6.png) khi click chuột vào đó ta sẽ quay trở lại màn hình chính bàn đầu (restart game).

- Nút ![](https://github.com/ngoctuannguyen/INT2215-Game/blob/main/res/Screenshot7.png) khi click chuột vào đó ta sẽ quay trở lại nước đi trước đó (để đi lại).

- Nút ![](https://github.com/ngoctuannguyen/INT2215-Game/blob/main/res/Screenshot8.png) có chức năng xoay bàn cờ để thay đổi sao cho người chơi có góc nhìn dễ dàng nhất(hình minh họa ở dưới).
![](https://github.com/ngoctuannguyen/INT2215-Game/blob/main/res/Screenshot5.png)

- Để di chuyển quân cờ, ta có thể nhấn giữ chuột kéo thả tới ô mong muốn, hoặc click chuột vào quân cờ sau đó ấn vào ô địa chỉ cần tới.

## 👨‍💻 Kỹ thuật lập trình <a name = "programming_technique"></a>
- Lập trình hướng đối tượng (OOP)
- Xây dựng Mini-model AI trong game của mình
- Pointer, vector, hash table, tree
- Bruteforce, sinh số ngẫu nhiên
- Giải phóng bộ nhớ động.

## 🤖 Hướng đi trong tương lai <a name = "future_development"></a>
<em style = "font-size: 15px"> Kết nối trong hệ thống trò chơi </em>
- Sử dụng kết nối mạng máy tính (LAN, Router,... ) kết nối người chơi với nhau.
- Thiết lập một trường cơ sở dữ liệu (database) lưu trữ trên máy chủ (server) lưu trữ thông tin người chơi.
- Xây dựng website để mọi người có thể truy cập và chơi cờ với nhau.
- Tổ chức các giải đấu cờ nhằm xây dựng cộng đồng chơi cờ vua.

<em style = "font-size: 15px"> Cải tiến thuật toán AI </em>
- Cải tiến model Reinforcement Learning nhằm đưa Bot chess lên tầm cao mới, trở thành đối thủ hoàn hảo
- Cải tiến thuật toán bằng cách xây dựng tính nhượng bộ của AI để Bot vừa là bạn cờ, vừa là thầy cờ. Mọi người có thể giao tiếp và học cờ từ AI.
- Bổ sung Data là các ván cờ của các kiện tướng, đại kiện tướng trên thế giới.

<em style = "font-size: 15px"> Nội tại trò chơi </em>
- Thêm hệ thống tính điểm tự động (Elo).
- Thêm bảng rank của người chơi.
- Thêm nhiều chế độ như cờ chớp (Bullet Chess), Cờ 3 người (Three-Player Chess) nhằm đa dạng cách chơi hơn, không gây nhàm chán.
- Thêm nhiều âm thanh bắt tai hơn và hình ảnh đẹp hơn.

📚 References :

* https://www.chessprogramming.org/Main_Page
* https://www.chessprogramming.org/Dictionary
* https://web.archive.org/web/20071031095918/http://www.brucemo.com/compchess/programming
* https://arxiv.org/ftp/arxiv/papers/2209/2209.01506.pdf (Neural Networks for Chess)

![](https://github.com/ngoctuannguyen/INT2215-Game/blob/main/res/Screenshot2.png)