for (int j = faces[0].y;j < faces[0].y+faces[0].height;j++) {
			for (int k = faces[0].x;k < faces[0].x+faces[0].width;j++) {
				cvtColor(return_skin[i], hls_img[i], COLOR_BGR2HLS);
				split(hls_img[i], hls_image);
				if (hls_img[i].at<uchar>(j, k) != 0) {
					H = hls_img[i].at<Vec3b>(j, k)[0];
					L = hls_img[i].at<Vec3b>(j, k)[1];
					S = hls_img[i].at<Vec3b>(j, k)[2];
					sumH += (float)H;
					sumL += (float)L;
					sumS += (float)S;
					count++;
				}
				else continue;
			}
		}
