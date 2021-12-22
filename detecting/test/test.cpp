//
// Created by d3vyatk4 on 16.11.2021.
//

#include "Exception.h"
#include "detecting.h"
#include "gtest/gtest.h"

#include <opencv2/opencv.hpp>
#include <vector>

#define MODEL "/home/runner/work/BroBand/BroBand/detecting/model/annotation_resnet34_porn_model.pt"
#define TEST_IMAGE "/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAMCAgMCAgMDAwMEAwMEBQgFBQQEBQoHBwYIDAoMDAsKCwsNDhIQDQ4RDgsLEBYQERMUFRUVDA8XGBYUGBIUFRT/2wBDAQMEBAUEBQkFBQkUDQsNFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBT/wgARCAC0AUADASIAAhEBAxEB/8QAHAAAAQUBAQEAAAAAAAAAAAAAAgEDBAUGAAcI/8QAGwEAAQUBAQAAAAAAAAAAAAAAAQACAwUGBAf/2gAMAwEAAhADEAAAAYMliRd4J15p50bjoOlpHxIcXEkiqpA8ShCq8kNPc+e8dtnrne5fP7a+svJfXNJgRXu6OJEJEhQhSESFERMEmwcAObBwEQEwTkbcbCbfiR+WW5erLOeF51lZI5JxpJBlxIJy8ki8qScqJQPPt/j6LZXdFfR6fXeXe6+H+yXuJsEVLjLcnCkoqIKCqJIBCiAGCIAYIiBgHCBgjUanH6vzm9ob6omaCvlxkh6avn3GdvUJhAbo1VOS7uRJUAUQ829F8jptVvc/JbpNngvbPJt7b5LXJyXuP5tWkV5pAXkAklThSECFEG3Gw5AMARbNtPq5NNFzdro7PP2FvXzoTzvbzxb6vtCySQEWGocUoKAXS4NpV6Cn8/8AXaPO7hWrgWSedz9VTuiW0Yd02FBo2LGkHkRF02nEiTuSADAEQIARAgDhbNtPzTcVM/cwmtFs+F/n1vu66udFuMhbbyhuypJkkVkcIi2SMYk051c7U6WXKz9pntpLUKYFqmkUcsexehP3mR5rguM13ByLpNkg6g8ggEIICQByAQh7QGCODRiuymjmarJ6Uw30Kod6+K4OBLvOKTLiOFtisdTHIdjvFrkqE5WaGv2eS02e2j9XNq4xW115X2NZJJouusRES+yPdyIk40ZDiioHCoFCBg1wgTYcIE2n+ZVTB5zTad2piqL0Icjub6nbta+2k5xNVLHHEdTOXhQWRDfptVVarL6ej1Qxjbjc3AnxezgZer53XyDyJo8KqIiRk0pDxUk8PmBwmPgUGuECbTkBQDsdSbHGUV6kfRNSNoN1hL5608qO1aVVnJqJxZZuQHTHM6IaDzsR6r0rV7S4Wo0fpzHlleyT2TLY31ZCpsa+dYVaIjd/iFZ6In0Xndhkq++fOPYc/bpvTfELOeu9h7PWvdTyhjqWuCiI4aHbUlNdX2qwul7eOM5ZOTwLBsqR5lv5Gq5rrZUOTrOa40thlNDzW8n1zxv0yCTR0cqw44PnZveUfZW+p2yx+Kxzsoa7olwqML0TuiPObXVGggiGBMrm585blSOA2nrfiOv6a70g4L/fSSFb5DK29CxltG5Eb6KPWSsXpNHzN19WxTen19YMeatNG+bHc3uVMdG0uvOXmy+q6bzv0mHirKLa1ChmQrFtrsnnPS/FujpcOm4dt2/mgdBqMs2w7lZ4R6c9cWdPp+XVwq7TNui1z1VY3fmcl2I4+HCTLH3OpvfCB+lYMw+XNJ9FYxSeAh9T2Nds/ieJ9LamXj+P0+wNEYPiNz7JlqX4tcu4sNmnrvkvoEUVhltHjmQb2C7FaMLUWMGewaR1S+MExHQ1wTq+WpBECWrs7aluOLXE36VvTF47pt91x5/gnPTayTn8wid2e1Fpad0jJMfu6OfJp3ce4pXu54mzO5zeXun56Ee5Mb0/c2C4xfdzR7BvuYMMndYdCJ3IIHc1zELuY+NH7jzP3fdD3TpfcH2Gu7rrzUA7puT/xAAsEAABBAIABQMEAwEBAQAAAAACAAEDBAURBhASEyEUIDAVIjEyIyQzNEEH/9oACAEBAAEFAhQ8mTJk3x5K56SELMkpRZqaJqVwLsPxv8DJuTIeTe3XuZ/r+S9DHGOWxoHFwvdKO/8AG/N/aybkyb26994umnh5CpNDkpLFb6oVl4ieDORl1h8T839pSDEwWZJnqSObMmXVpCe0yb4L8by1MPK0Knk1FJMPQ0vqMjQ/5/ifm/sdSE0k0MQgF4vT3Gfw5IpFHIgdN8Exaje4OOvz12kV6y1WvU8NQLqrc98t+1/c6dN4uQltZSL7ov8AE/w7qJ/MT+/a2pH8ZRu7LQqOcOSqN2Y4NS4e4Ix8nW1tbW/kn7crQW9TyuVyf8IvxpB+0Pt3zCDrCzF2xp0/UTQ1REZIlNSaOWvi69iOiJxCnT829j839scQ2BjN6s43o5EMyaRl0oI/INr3uu52oO0xI8fGE4N9pApIxNozavOXgnTp/lflTijniuwvakLF9sYRsxseMmumePsVIqLTjF1aXcZdxMa37JHZ6tZptyKNWoXkiEJYWgkb6hKXnaf3vzfm/OvUbYEXqL8zvLDj69etF9zZUmjgpPPGXfQy+WNMS6l1Lq5SP/TiyzzRwDISNn1NZniUl2SVHuO4flO/s37nT+5orlFHKN0Yq5VFFfcmLKaD1TzLabym8IDXWutMS2trf9XCt0Sf+P8AiV1KHm1F/A/mN35b+eXKF0vYMgilllAT6wdAtoOQLaZM3Oy/RDi/BsXgi8EWykHxP/wx/j439rqnaqUhisFI4fynTs9p3iXRpRDtxjXQmZaTMtc7f64ov7bJy2teSRN3IKT9cJ/v87qKlPMoAIJYLDxTTTabFZOeS6ZMon04mtpk3tuv/Fhx2mdMxuZcj+wq5dqc/wDT29TMpMtUiKK1DOt/AJRNF5dUw7luc+zZgmOtJDI1iNvCaRNIhJMS2upb5ZAumtiZgESPSKddzaKQYgkmlyyl/wBC/bk5J5Fks8NM7uWs2lJNLOYdbtiM5NDKM213F1rqW1vkBQ5FpI3rZKpC/wBTyuOfqby2GPRktoHQpiXUutda6lk/NQ8fHPDczmRxs78TWCaXPXCWJrnmLT+RkbqH8xp1pTeGz1nUvcFCTb6nFOCjyc9RY7JR5CHa2trfIcLL135nrz4mXvWXU+KikWPx3pSIE4oUVgImlybMp8uS+pzSSevmXr5lCHrsTXb+tmseGQq/dGRbd8TUahRfwshBLPPnDeDH+rlXq5V6uVPakU4tOpKulBCwoTFl6wekmGcad16c0c7Sxsa2traqRW6w5L/fD2o4jDIwyvtCS2rN+KFnyByqS1pTXNo5HJVEMBEuwSo2SgwtWfRy/cuIMSRFw5Se7e8MpDUt0Kb8QSbrMO10Ltvrsuni0jRfandyWlvw4Lh+9otpiW+QQMzBVrqyMZGf6UZO5WM+2JwWLampFCMthHK7rad1WPojgyHpwgv/AGY/INurMYE8niXy2I1FL16Yj2rLeuu5i/3pYp2AxssMZ22dPe7hy3IijIvLOh/FcWJNALLsg6Ou0EscrShtM62rTBWWUJ6Veo/dKeyDPQyZRzXfMLkjUn55OgP7etNLpDMsXcazYYe/lLXargQRd2qPWIMwZSNggs2680J9S6k5JyRF46k7eX8KsbiTXtoLos9y6E8NAWComdM6djs2OIpuqPEixVSxEZWcpTKsTuRVNon8S/tzHa0606ZnVSYq9h3ZwcWZoJe1lj8ifgcm/wDYW1tO62nfbuTivyiVf9traP8AXAPury2qNfsjg8Jjb2GpcPY0Rix1ArjYeg9rDY+IQocOY2SrV4dxBBwriKG8LjuHcsOQ4axUPDk3CeHjtz8N4kAh4bxL27lXsXGjXSsbP3qJPpTEwXnPxIXi2/csdK6V0+XFdCMEW05oi2oH+5cG8MV+JCm/+a0GCtwPRrRTcNwx5wOEK8kfEfDcGFrRk6vZe7ULF5m+Vdr9kZgy90Jo8rdgkDN5CGL6/kRX1q+F/G5W5iWHNX+y2eyLGOfyISx8QZKJ3dydf+4ZGrzfyxN1QyrpXSy6WTAy6GXSiFEKcWTioPyq1+zSUueyXZweTuHE+TuFL9Rt6O3PYX//xAAwEQACAgECBAQFAgcAAAAAAAABAgADEQQSBRAhMRMgMEEiMlFSYRRxIyQzQmKB8f/aAAgBAwEBPwE+hw7S/qbevYSzQVsvUTWU+BbtHb0lqZ+0ZShwfPwIDaf3hUzi9Y2hx6VAGwTVr7wCN5eD5QGMTsnEkZ6unpJmsFR3lxZ/mnaN5ODUpbcSwziACZmAZxelar8qO/otWytuzELWjLRlwssX4ptxMTE4GcXMPxzzgzjn9RPOOVYbHxy2zw+0e8tzxy4Iuby34561ihqP+U45V0SzzjlsAOY1asu2WVmo4MbycC7v/qDry1lPjUkDuOs15F2iLj8HniGth3Hlrs7g954v8Xa01NeV3TvCJiYnBejPK22oN0NyQE2D8S0fy11Q9v8AsxAJp6lChgJjPeX6TecpGQocGY5JuZupzNRWxfpPFfbtzKqrLDhBmVcKZuthlXDak9omlox1WGipPkEZR2hrIbbAMDAioCes/TU/bDp6fpLEqVe0b4jmBGPYTVUbhu95jkFPvD9JTQp1KLZ2Ji6dKl2oMD9oy4PIDpNgMcbDiBwYWAyJQQ5hWMojgAdZ7ymvKzUaVbF7R1AYiYlbmyzap6Q6Z9xYNBUysmT2MbiSn+2WagZ7Txx9INWB7RTLesEMTooEzDNWrntNpz1mm+WX6qtd1Z7y/TMz5EbS2KNxjqqrkCYBUQ94aq/tEqqQ7gVjU1/aJ4SfbDG5HkDAeVoGJpvllyKVJxNQzCw9YXY+8//EACgRAAEEAQIFBAMBAAAAAAAAAAEAAgMREgQhBRATIDEiMDJBI1FhM//aAAgBAgEBPwH2NVN0mbeU2dwOxUD+oyz7U+qh0/zKjkbKwPb38RPqCFBaJ25b7XEHOdM61wSQlhYih2671Ugd6K0pDX+1qIGzyAyDY/YWk00emZjH3cQkcyP0rJWrIWhkL4t/r2Rqo56YButTL0H4xoama7tMdbQrV8uIi4gUUFjkFw34HtPZNDHncah07dSSxxohQcMax2Uhvt4gaipHlpBnmP4uHP8Akz2crAH6THObJmmPEgsdvEfDU/Y8tLJ05FpwY9Tj2ZDtkiazHfY/aYwnThw8rSyG8T268WGqRoe84oQlECM/1M/1jefvnK83V8o5sdig69xz1LGlgoLTvY2MMJXTYDkAiQPKM4HhOnK6rlI8u8o2HWg8EZK7NoE7LquXUcrJ5xPxNc5AZPhstx6APCa/8NjyjISbWXMojIrpmk1n2qNK1aHIupNemmxy6WMaBbW4QIANLon9rFYLBUnBN5Uq5M5PTGHymSACkJAdkxzsvKydflAlZFOJQJ5v+KZ57gnppNqIelUF/8QAPBAAAQMCAwUECAUDBAMAAAAAAQACAxESBCExEyIyQVEQIGFxFCMwQEJSgZEFM2KhsVByghUkwfBDotH/2gAIAQEABj8C9xqOI6Iuue8/MCrXG4j5le3I8x/RXxj8iPVBoa0BEtoH8k+B/wAX8/0SY/pKxUrWl28OEJ8llC000orC3e50GiJA0fcgRz9zq5erZl1KcHcYPtpGjUrEMcMxQoVbRx5BUpQou0DckPc7EKCgTXtOXP2xW/mw5FbSOQBrkaVcQndapp9zdVMHitfomeXtR5qV3OuSbdkrW8k60VaChEXeGfud19HDmmVFbaElq5iP2tUXck95zz0QyXCiaa5olzAfFbNzi4DSvuRLPVv1yNWlbSRpYzh+qq2RpHn2Z+zBKuOfRGRm47w7aHRbMZh6afcmuhcN36EJsUrtmKVqrmNvPU5AL82gro1NuDn+aLWyuHLiyCAxBa54+JvP2P0QDuBop59ptNCiHmqN3ypvn7kdhM+OutCoWTS1tPE5MhurXNVcfGpREfqweZ1KDG0qnGWbasOgpw+xctxpc7Q0V0n2WS4bh4KlpHmo5Pny+yZ5+5b0d7PmZnRC7LwKdM5xaxvBXmmnEFwd0Lcgtz7LePsnLEdLu4SoHfK5D3LckkIOt1NFa2Wra1GSDpt7PdKqsvZsCxA/V3Cv+9UPcpBO3aO+Gmaq1gGfNXgNYWjNvJFsnCefT2jfJYv/AB/57ssfOhomo+47kZ8zknNcDkhbkaFEjMprJXl7HezBU8nV1Ox1QLe2qfH41CPl36OnaCvVysf/AGn2LmtrHKOSI1c469Stm9tBmnsi3KZV5lB/FTqg9uh9kPJbGvrKX08+4XvcGtHMoiKsWE5y83+Sjf8A4lNPh3dm0XSU5rekoOjcgjJJIXuOpKy5IRTPLozlvcvYOa6MhzfmUdSbWyA0+qNdLCtuwf3dksZ8wPZCnMJjs2SM4JGcQRinEcnR9tLllHED9V+Y2P8Asar8Q50sUeZuOp6LoByUnhRyjPdbH9e20N+qqUN8vb0dmrm5Eat6d5tZy+PmOasoKA5ZUVx5Rkf+3YbdyvROcTc7QHw7m85bg+pXF9lRshHWi/Md91+Y77qGrt5zBn0UbdS0WlFhykGbHeKLHC1wyIQAzJyATIvi1cfHsiZEaZ5+SoPALjK4yuMriKq7MrJXO15IklUFVeDk0c017Muo6pr28Ls+6d/aV5Feu4ypSaigy6lUqWn9XcPxn9KP/jHguq1r2Eo0zpqjkmnVzK5fVV+CTMefZt4c3fEOq2jhuRfyqDsD36vNrQmt6u7P2Ve2g15LqVn2ZgrJGB7sjw926Zgd/KM7s93duK3Puv8AlNryyVVnLGB0vCJ2sbvBr+6Oqo1UI51Rjd8SOHL7oC7aM8D2FYho+avZ+69Im/KYcgNCUGVqG5oedUR+u5PyGbqpxcPhonbgBRH1Cd/KrWngs1wrhTJAC5gNXBBzTUHuWirpTxlYbrI2tDnRG6tuipUeQQjLase77I+HfHYVVRs+LU+axQyd6yQ0zz16K/YtcaR7r6ilbq5V8FLsYWsc3EyRblTc0CtT+6kdZtLaZBpcftULFRMZe1u2a1tdd11FhXOjEcu2w18Vxoyr3cq9KFXSwGC9zqN6UOY7D3Aei3lRBbzW/ZfltP0VoiaPIKO3PKvcbT7+ChaWg9CqkfGSi7SM52hXs/KrlT4Vv8Vmfs2SfKV4FZBDlUEdhAVByHfoq80e47yUmdaO07hJ4isDLisJFLKHSvJe2twDi2n7j7KZgwcQEk8o04adOiwMWwgJ9E25Zs8yaak6EeCkZ/p8JvjicDIysVxrlTkvxN82CZiJYpY2NgcLtX0IFf5Tg/8ADGRGV81zZKOezwBGn0WGc7A4e4xBpFgzJbdX9ivwnCYrCQYifEwyYyQyNuNujP8A6pcVg8FhsQ0yRwuJh3a0ztB01WKswEG0ZBiHNdZv7pyN3goy3AYYh+IAc3ZjLcOX8FPswOHd/tJHA7Ia9VjGH8PwtPSgwDZcti00HRYiMNtDJHNpWtM+1tdW7vYx36u17vHv6KlKDsHZjBPNJDsQy3Z0511y8FJTFYnjoOHhqwdNd5fiDocXiHCF7miha/gpXQa7xUWDEk/o743SXutuNLtMvDmFKRiMQKONtzQMt3UU13lHLFPJKXy2UfTSh8PDsgZDipY2Cota7qan9wpq4yb1rzfvnNRyieTaRi1rrjUDonyjFzCR/E685q9mLma622t50UsTMZOI5SS8XnPqmkY2fc4d85ZU/gpuIbipGzNZsg8HMN6J7cHiZMOH8QY7VbD0yYxWllt/wnUIu9NnuL7ybzxUpX7Jkgxs17AQ038ipSzHTtMrr3kSHM9f2RJzJ5ntlHLtafDunvjsf6PiJIL+LZvLaqQen4mhN59adeqxLDipi2V20eNod5x1JQkOMxBeKZ7V3/eZ+6ePSp6Pdc71hzPVASzyyitaPeTmv//EACgQAQACAQQCAQMFAQEAAAAAAAEAESEQMUFRYXGBIJGxocHR4fDxMP/aAAgBAQABPyEaAhhgglQJWla1KiVLLJzNjywRKNtD7SpjkDMrRQ10sqVo/Q41dDGMdGLDNsDWJsgSoSq0BA1UBXYl4nF79HzvAF2bBBVGZEYOqoD0/wAyvpdHR1MdDGMdJxDXIYhAz/4P8RYloixWNVEHggVb4ZkW9jXyiIjETk5g5sC/oYxjo6mOhjGMudRGwcK85QmonQsv/FtFH7xlB2bnf+IBFoIFy2HdItDbT5zvFj30WXFly5cZxqY6GMdF4bGIbElVUXwwHMNCZGUzFGWXH1rixGhbxMBNhftn/eYOCG2n8dTFMKLbz7l0bZvtPNZeq6F0XLnEdDGLcY6gzVZsmW5sbbMOi+gj+z/EVRbKJuIQfoWOgVDs/wBpYDh+T/faKjQl0Yjqt2uuWWX4K7/mBKs5MjpcWIw665ei3GMYxjGYQOKi4CuixEaqWygdzAA2guHKHCHEIOl6FizceXaE5sP1glbYi4lCbIVth+I5HSp58TfVNbEnJ+jN0WlgxQfpOhjGO0dP0p/tDmVU1YA/GdOoWAjZOUD2wvA0w0uLcuMUpsolMy/0TMG78H3FCMAlfxtcs7uvTCFd/mbPpCDejHR0Y6GWiFsZuZzW8QNhLgLFdtd/nXMH+gtpY0BtxhtcMrqTG0i3Z5xAQQBoe8uLGKbMRfWi+ShBi72uAFpsyun4Yw8sK+7PV0jDjqMIOp0MdDHaABgxdpz9Ti+cwY0UtUoWSvIcRhUxuO8dQcEHIbwZOTjvh50WxdMGkgi5tztBAEaBszKlMXuwmMt3b5+yahdLgj5f3HR+GguYsWtBDV1GXFjHaCtxF/dPMt7T0faLbILmvqVwBuPQQiGJ5Wf1FBY1sXtAhFCtpbI6AhhFDzL7W3+JZslJqwBBORBTmVt8N/1PhKdBfoEIOlxjGMYxgaJFZ3lUVVzJ5d1x7jr4+w9yu3QA4B6hpgmJxibwJLVAxGBphBzmMF6P6QoohscskKR0jKv1TbfiLf0jqupjozZK/jhDP44lVHdvkVVQ1qqhd8dQKVZYYtybMevQhraYaZoTRcziSi8ZQmpsogtHc43X3EBO+I8Hgdb0uDCDei1GMdox0ccvAP1l+cstbMyw3g9MqgrwfmGsRKQxi7/SJIEDoiWO2bIRxqJ40tVbR9B/bMFSkhwTeGmO8NJiN7HxGZepo63KCrQQM/FnqGX6FdCxYsYsZcTa0d9L03ASxr7ZUiX+nK8mJmv3lwMQeDn7x4L/AEJd/R3ZDGMX0uXhOk3fuic/tAW1SnEboBSLVoI7SOEK8fU8xVzN79T94fEZuXAIQRMXIeB/MwqbvMTuUGljtK+Ds34j/Hoq1++pyplEsp3EdxtFgrX47X5IM2BWv/Mzs4xXsIp2MYH5m0fvLAZx+Z+0GhJiaJNTmU70hPuAIluQVf0+GdjWlR3hnsCG0Ex/oc3FqeyycA6lBAMQESlvUl+TIx3oVtS8GireZdzGAuCH5SClTnept6PcnQNNvHv9GZ9mbF+sYYapsYOIu8NxFOomkv4INn0DBKmOmYoD1zBYqbzfsIV8t0pj/caQvjGCuVUQc5Y7TFyh71B/BlJpdjhjCrVDlgyBr5D3gixAc0WlqqE9z/oT/sRT+SJ/mjBhOYFoK3OB/MAoxiNL2JhByunubYBwbDqMBYUfpBa5uw23qBG4ANH9QZ6QFLl4/wBzKv6o1GaSJq4qFlxBiTJQLcg3ptEf/CLZfiFDdtlSHhQu6GoLv2ApQq67H9EAEY4OnY4fzO4ozACcEpfUQLE9iN01+8/VVBp8xkUwNR5So1lIbVDC/KVXD59S13N8RZWIUpRZVnIndJ34eSGiLS5hUN27EZlBBlVOIOaRO7MM5WzdBS+WpnOmyPzGRQ4Cy2y/gnI6aCWA2GPeYz1VVZ5/28O5MZWPyoFD52jA17D2Zi4O5hiL0QFXR6/qMDrANOCWNdxtoEgrgB4xGECLV3EJBDejCC6CMoTD8QbYEDO0MqMormWj9nJEoJujCFWUxQh9D5nbIgpkl5bNO8qnOxhoyLgwRTr/AFfeLDzCfw9xHYi7MWTbYPMyvAwHN8TlawYsdkwcXRYmL6jFzyijRpjjswZ6b18ZmMhLO2DsMl8EDAjswmMGPjeILd1ZKZYuTkVNjdHcC+GDG60fMMQR1YgC9845mwKwrF8GGba+sRkJsYgqAvOHGY5H0GPDhyQA7V4Irg+5sxCO46yvRIOUHTDh/WXNLJURYl6wA71qm0X/AIWdO0bkbpTgFvI69Qlh5jdD3KH3rVd6OlKQgmSE6MH/AGPrmBkzm9xpRGpO7zzK0JZECCoWNsQxsRpxGUiuowG0wDA/MBKs8IjkY6iwm7QpHu1U+GJcGE++f48Q9mkVcTs/GjOKNbQa+2bRxuC1272B7FxJYGxfIrRkIT83Qp8RG9hde5RK+NStJOHlFekLiT5YPI1FBUOTZLp3FAGPBQ1fyVMRuocq/wDBcwyzKV2nBijvMrMiumKV7EzyU4a2M7K/fuBUgHEIq+dDA2lT7b42/SVmK3iLxp/K1PtiYz0gZywvKcm8YANku3OibAKJVoN+kJAbWyXEANvPXAMv3M8Ywzq1IQoQVsFG1VmBKcEjSLZsEsG94szqWQ5iW3M4MGJwu8CWuBm4pbwd6EkAAD2D8QWSM53mHFlw57B6y4gyhoLOl+uOphKLqqofdfuzdaoGu7557jSk08Ij+ofMSZ7TwmPWABvYj+3mLSde0n4Zg7LfSf1alQJ++rG4esH2i4cAjWFvNA+CO2utyK6Jamb1h/MNXAW9MuDekwCUeI9UbNpYwjKtQMwKioUUiEDBOEdrrfdmwsy3TVbvB9ibbuiGLkZWi/UuiEHZi65/6O48Gl9flnfB9jqFDGkf5Uu+Z//aAAwDAQACAAMAAAAQ2vw2qI/X8U5owDIt670RMBG9i+5/76zQdTzf6pU9rcJDxv7opAf9wFbHoeV8HZ/xOsWJcah3ltB/6Lfe4DnGqg9Wz7LKgI2/9lSxzL9PGnPKcn2bP+gy9QDsW9JxBAHfJf0cu+H8VKlr45nAFcWpWUMHPQIApn/jWgnR3dMHywxfXBfrbvZ26m2lIeWytBpa5cSwyuotu/eJARMOmbQni1WO9rdsAjdieDjhg8/8CejdiBh9/8QAJREBAAIBAwMEAwEAAAAAAAAAAQARIRAxUUFhkSBxscGBodHh/9oACAEDAQE/EFcY+neV4eb6JVia7QX3WT1ENc2Yw3oxxp21Vnz/ABKRnEb0Zr8OhpUCENCAo4gFaAVK0YRN11IxIZqYWySoEqVoQ2hmBUKio6P1ErFVKRbFfoEVRi+bgkoiusUbQXSDeOesNa0IEImNjPvKyFQuxM9E/EdQ7o/BP7KWJ1Igb64IfE/MCVqQmzRaxjkTZAcYiRLJSIRTtj5JOrLraCF6DyJESbCj+cnxK0dTUKDK95aBiH81y0zUdAHt/aLJiRuAj3M/5NhhP2E6RJZ2g9qHtAgcQwQIljTj/ImiYT9wOYQKSiJ0O28H3Lb1l375+4MxmOsV8oAH10dmh9zKWtVMrHmZMIVVTEJU6KqLBx0ixFhFU8JYj7JR0uxM1+8eUvEFwF71KV9p3Kh9IgAG+87TxBmT4iNRRFYCoBaMxMwjSVDmxlXHeAXjbd5VwHQEJYuIaFm4ICI7xAanKBOK1AYqPxYlDQ2gg1cW3Sje9oSdhjDXCHmY4LjXY2X7ZgVW8ka6w50gFWgUN7xiGAWnQqzpG5EpiJgVIAAVFJFrCtuSNTKaimijvHAh7EuCcQAa5ii3wEKATHQl1+Ajs08EIbTbDc0C1E5iXvDaXmJtx5m63o4ggLzAEV5n/8QAJxEBAAICAgICAQMFAAAAAAAAAQARITEQQVFhIHGBkcHwMKGx0fH/2gAIAQIBAT8QN/0PdkvaFxR2fG48PuaJ0m+O4yNj86LDFfvPA3F+pD57Z3PeVjicFMUXNy4BC4KUxH9u4ZI/BuXZNcGvPJf3HTL8Xe2PggV8AYqtltrMI5gypO1VHHwWiXfJ7xYw6jqjZt/16g8/fmMjXJdz7wlRm2ZZK6mBvn9ow+C+LlYKE6jS9k0sNHX5h4OTEMV2kfB2fKEKd54q4NzUY8XG41/HcrQz/LlZoErjMz+9gwfURlMunD+ZenshfUvuagrQxeHhCwcQO/uDHs57qD6zNQWXCUL3PCjH6YlnMwDP+ES/9oY4WIeqXWonZgGyXBgQhm8wxF+LgTkQ+1UxEvxcT7gZbUVJuIYWy3LvrqHlg3cG5hiUTONMvilUjvNxsLEL4QlmKv3LJLuKkWJrGULMVBO4kFliCiVxNE1plq4wrBlCHaxYMsIdGBQNC2WO4t7ipWWS6pAYJIA4QZdxRgzF0YRUWBHUVX3EmC/WOMsLdsNm5SMwXjfNM7gFEQuIR1FNoAl4gJshZqf/xAAnEAEAAgIBBAICAgMBAAAAAAABABEhMUFRYXGBEJGhscHRIOHw8f/aAAgBAQABPxClgw8TN8ILAePgCfAQFTCVA4gmVrv1+AAoEhw6nYI7OSED2HEt8AwrFaUS/O4adYZt83J0/wAQxMX8C0SmVcxes5TKG3dQ0wxanhLMTIfBUTNILmvyBZ6fCnG7nlGLmyVFuMyAWroiFILBquw8Nu/Ygd9AEGPTQnoXo1EC6MHbXmvgp+KIlTLEqoc3HE2Ytzh8nCG6+G0VzFlkn9QVNILIafCwZnlKuVUC4lyrgVBbUoLN5R/MdDUKdgMdJZGCy3iskBES0e9JjTQeUKr+Ui5UgetkWo7/AMQ7Y7iysdzhMIrZw+AxDbDqPafg5V6BARgXcp+IK+iRRWijpNCOj4UFRFgxXcC4lwKlVAuPxcPTCvB/iGdOw8wV9j7iINUeXKahzjlVfmAwvM4Y28oeiIKVHL0i1Mnw7HwC7xG7HZHaLc4fJw+ChtmFSqo13SwXn8kXJAWhex64uUA72LTSPEQahY9SGYYqYRp1KvNzVhv4W4tfDFVYmMv4d+PzUYCmFlpdHZB8QTKmbDv3O1Q4dk3bSrVncxS8pzfX2wgG6NOEE/cSo6mKNeIsbM843l2ppc4fB1X+ANo6uU6w2nom+/8AqOwV2nA8OoA/gvaX+iMm3YbfhNTuIaZSDzBD4X3iDfzXWJfWIgiVsSzqA/sEulKIOm1eVfUAkkkf+ahjD7vobb+oVnRhLIicOmh7IBlnFFhATuUjrKboi2CZI/CVcfIL6+RanQfIc3/gMBlQYMAuOiGajCVI+57k5NVHbSYrVF5NbYUAAKA4ISdpcsYjyoEUrvHy8JRFMwbSOA8TKajeVQgebSMKAdA1bXXf5mAWmJs76sMAuaIYVZ8kidm2NqtRHYlkBWgHYSsX9xbm6IzmIuUMogMGo6mzEqazhBBDi4qgi1FOnFA6dQpL5m+jy9gOBe+iQHKtLQryOvcrBQsR3FBlWhoth3WALVwydIoN4jbv5+QWZTDCwi5QgTIdDt37ww3NbTwv5nEWBY3cctK8xDQBHNneLMlArJbfePrvDYoK49H9xRpBaxg38OgqDfwzL6xamjFtmEdTGo6GYCxN0Uob5LTQQjsuSC65bPzMYKDHdK+tYxl4hFdLDqTwVR6pmIKxiDyhWNe4WiAfZC7SsY46R1rUY9JCKfExNjAG5YxCR4jaEsC2O1gMSEI84j6fQppZVmGNyp44CVTtYOpkdzc2xphCF4Wz9ExA0v3w6nR9xuYsGpsjpl94nCOJvBVfBVNmKiK4pYDPAa4zznZ9xWQEQZChLGGi7z1I0EqMZtQ1vUcCqCGUGOu8BmNYLHrtDo9bvsMFTtVaN25ulmWwIGa0E0pDPSNOZSF/mEStkAGYg0y+6ZZpuLpiiZA59S2OCFVii65PuUFx0XUJY8GxmBH8xQ7j/EQE5aU4HiFQAHjkf+8Tqq/tlMtVsQfgYcR2xQesWprFm5wipjANsNEVRjJMegrhwrZQQsa6g8XVGsv25lfZKV8Qb2G8Yc6h/nYctzSYCq03jNpFC/FeJs58Je1YIqEGVrtL1RLDcfBuGCI6ajqXzFiO4VMzoHxK4uqr61/1BgnEa02ZuWlJcXUqBdLXpLklkcLb8kIoFtT1mZl0s7Mtzn5dRUTB8Iv4YrH8NYorGZFWNDsB39TOktqrJWLahpZ4GPeyGDApiGarD2mai2GupuI8p0VEJlaF+oTJRKR+CUC5yowNRDfwAQIF7/8AYlOvtH+oAmcSwLhEGOsqmr6ysuteAys9corcGcPr/ADUGM5cwbgjMVRbmzOc0jFZuC4a41FU6oWE7t6y9ENUyZBjhcjzjVyqnu6ArejDq+PM2VIdirNnRohozBYyjVpUnBGCDmIN5IQalw4rtACah6QDHQA+ou6oEdFp/BDBu46XFMVWORYqnY0+mO1tW7hHJPsX/XwsYX4UZjHUoRb+AqIquKlHmoqY6IKtKYwDzSId5HSoMP8A3SUbBwSiW/cK0iUbo5UXyHzUQwDs9zq+DEjbsQ7D8QDaVqK0Ui3OIVf1MtS8PNq/v6pVZuNVRzHKIoN1BgczXYunuO5ceIR5zleLr1Ha4+yr/uO/gtS8XMYAmC1WgIQ4ViorSIVXfUPDHOV+hnhMuZj6zFHZqpaxWx2mWhjOwWl9v3ujB6W2OL5VmcyNEAYV0zCIML32KvLrsn3EPNs6KUnI3FliyO1yPcmauUBmvM159y3mddGMGJ1qKSydr8j4hBnI4VwHUEt0ouZFYQ8OmXQZe8qhuaDusJl60dSb4Xd4i1zRUba/gi7kqvsjlEVG6W/HwCtEDWDdaFX0dyClsYDmYstX2soYBZxigt6AEbJFHQQMdxlwyLLbCOt4rKjS4V7qchFodVWhPpLL4ga7XhchhLTDyR5u+E3RsXLw9wDZ9E1fxX7lohQ2isH1w+JnHXd1qMmCwPSx/mWPUtWJYIm2MZcS43BGdopWH3AOmNUFzatwfuZnuLmmMOlVkEekxNqyDjGAepWH0rdUclHqyUa43SP24NlYnRpOBy1WMcyjM6CgA0EMbWeSXf4K9xuXIeq/1EdZQqIu2xipVQaIVVytD6p+444YfSW1cBSNN7DvLuym3Usbe4X0HQXJ6ZZxrKWjryPD2lOLQfiFtELEVwgAlQvkVSCl3nV1AoSdTAxVPGBZRCwK9bfxDRsPnmOBhUAh7EGuKoVwbTrePXeFYl6VKsSBO0uV6IqMPK1+D+4OStwq/qEjPJSjod2VUHO933P/AFcFSjyKK2+txKICu7e1X7mzl+auH6H+wlz49uTSMbGdsEaA9zF/R53F4NHYJoO3rFdLVNADK9tHuXNq8Kbp/uJa+1P/AFUM/lQPL+8OBFQnMsGIZomiDjSHqPUNGRKbtxsruEDhWlkPvMrzxANZb6Nh7JdXiIq9ldn8SjbIuRILi4XNwXmXl7SbavHJwd96lAyFYmgKp0IOtHUClpwFbh3dNGb96/MArpP/AEIxpAC1YALlJCHv+rldkdgeotRa+u1cfcTFm8GkVLV0alBNMrbmghjKFrIHn+onJo29+kAymE4swHpiO2BC6xC/Kqx2YGGE+pVtdbRc9wY7nqHH2iBi+jyZfqcACsRmzjaMGOQyLfa9+AiETl+l/EVlFrj7xMhYfdEmLMveVm/lk4hyAHMwoKsXzCuYktXyVU5/3L206KrPvUGyW0hZdPMKytlP+pVV89XLcHFPHC9L3XUesolW2AMZimIidU6DrfBAltFiS1S/NH4idTRAYHXH3OQPFUpfMt3ZS7xr8JCNDgKS93iHWXkIDuZPuLMzajHYGMmuDXF/uMt0OhG3UhLniAfEwXhuX5+rlh4DDd+r5b9u0e0C0M51nsYlMST5G7PlTZ4i1jEFroFbLbU6sBl1ZtidYfARNQObS9LsEPdzW4ncjKwyQdKG+1zH/p7wroPaucwTZ7F1VKvsNeYDoFRzUTbDFZxWPGI4E7GLGg9EroG6ZFZ/f3KEErn5Ps/M0iz0YlJ+WICJQDI5xfe/x2j4oAKbAq28Z171B1p1TfuBVd9WKA0wiSgoUFpOExcD4YLpNfWOQWXoVFm5dHfLzo6i4P7Vv7RANMDd8xaksqqod/Uv5Hxtg5rXuZybxSIsdeL8Q27JD6f6WJC1+4r5q8dYU94ubJbLGdohquXuv/YoFrd3DqapWe0uYCqRyeIfOUgBSl5UR1GMqYdhZA40kQuthbEWiWXoKFYFna9ag0Emm1kMVkBS4oKMelPW7BOFPYchK1wHlAJFQGLN4bbhzODIYirLGttrZVj9AcJDRU0S4ysDBvmY0edQEAaZQCLEUtzBWNJTjZC/KcUvDWeg1fmM1yLksB/MRcwFEscwCo1WAXrggA5sVMS0kAFPP4hoK4mLTUIoihuZrrLOLWvov8Ep8MbCCuetgRqOxTAtH6qaRJMC52OmA+8SzqklLTi97tGWCvvRWv5i7czMtuY39Ys/DgCCY2sXLWrnES0SxA50HsUlUg+GA5jPo2YlxPGRoInss9ygMNAJeoZEtlYhCELQGlt/VRn8CdQiBgZQsFlfaBkyazg+4dYZDjo+nD+ZY7YWDGs/qM3GobCO0fqUPu0jW1VF6yRu9aA1Jjuh/P8ADCKIYZleNKLuv/P+qCkm5SGRoaBx4CUawxEUWZpdrm3bcpT1fvDbsLlFo6I/AKZME5XaqFt7KSUIaqcpAFsMFSADbDCQlgAhSebjqBLgmRWaHe8sQCfa69sq8Si5W5Y76Z2BktMAveo2kOYCLLOCWKuHRtpzAZLZRS4zCmltymOwtaYEssQdxrwX7hzAzu9W8ouq8CoIs4VV8yimpkLDNNwv/HCULMZdEqTu1/MFw3ZuXbbmBsUPAB+oizCyrhwTCJioIqgLPEqICMgyjiYNc9W+8yLYih6QK155yubyzNM5IR0+3JL0pgqsvMqPjpsFgyQ4EKWyWzbeAQ5hzIFFWGOTpLgIAHZhpawYMxJCs2SWIbC4xtlLKrlzMwc1XEDvfME5xr7gaBFW0MOXjOSATnaYU2YZeBr9eZCzxGXGYepJsJpl3eRqrYyd2lFQtrYLL00bYc9DJyxW75B7rjVqxGRTM4rwSmIHYKAE0Q1D4eGKhhTVOBk6xGQ97SoLmkvrAHAKAAHnhX6AS7xpNYNa0pRGyw7CJDyQpqDeFPARQEVGBywbVXKvWUU4IFg1ojvMKudcIbJqIDdw9IKdsjXiEr938RXbZ6/DH0xPaTJw/cb2/uc6tHWWTUYFI+2tRm/rNYWLBxA1F5hgvVsHYH0mebqF/LcEGUOuhBGSROVF6IoqU0En1h/2rTi4JjMOTyP1Kdr0u0IlQURdXl6z/9k="


TEST(IMAGE_PORN_DETECTOR, successeful_set_threshold) {

    // создаем объект
    PornImageDetector img_detector;

    // устанавливаем порог
    EXPECT_EQ(img_detector.set_threshold(0.7), 0);
}

TEST(IMAGE_PORN_DETECTOR, bad_set_threshold) {

    // создаем объект
    PornImageDetector img_detector;

    // устанавливаем порог
    EXPECT_THROW(img_detector.set_threshold(2), std::logic_error);
}

TEST(IMAGE_PORN_DETECTOR, successeful_load_model) {

    // создаем объект
    PornImageDetector img_detector;

    // загружаем модель
    EXPECT_EQ(img_detector.load_model(MODEL),
              0
    );
}

TEST(IMAGE_PORN_DETECTOR, bad_load_model) {

    PornImageDetector img_detector;

    EXPECT_EQ(img_detector.load_model(""), 1);
}

TEST(IMAGE_PORN_DETECTOR, successeful_load_image) {

    // создаем объект
    PornImageDetector img_detector;

    // читаем изображение
    EXPECT_NO_THROW({
                        cv::Mat img = img_detector.load_img(TEST_IMAGE);
                    });
}

TEST(IMAGE_PORN_DETECTOR, bad_load_image) {

    // создаем объект
    PornImageDetector img_detector;

    // читаем изображение
    EXPECT_THROW(cv::Mat img = img_detector.load_img(""),
                 cv::Exception
    );
}

TEST(IMAGE_PORN_DETECTOR, successeful_permutation_channels) {

    PornImageDetector img_detector;
    cv::Mat img = img_detector.load_img(TEST_IMAGE);

    EXPECT_NO_THROW({
                        img_detector.permutation_channels(img);
                    });
}

TEST(IMAGE_PORN_DETECTOR, bad_permutation_channels) {

    PornImageDetector img_detector;

    // объект с 2-мя каналами
    cv::Mat img(2, 2, CV_8UC2, cv::Scalar(0, 0, 255));

    EXPECT_THROW(img_detector.permutation_channels(img), std::runtime_error);
}


TEST(IMAGE_PORN_DETECTOR, full_way) {

    // создаем объект
    PornImageDetector img_detector;

    // загружаем модель
    auto err = img_detector.load_model(MODEL);

    EXPECT_EQ(err, 0);

    // читаем изображение
    cv::Mat img = img_detector.load_img(TEST_IMAGE);

    // предобработка
    torch::Tensor img_tensor = img_detector.preproccesing(img);

    // прогоняем по сетке изображение
    Probability probability = img_detector.forward(img_tensor);

    EXPECT_GT(probability.porn, 0.5);

    // блюрим при необходимости
    img_detector.blurring();
}

TEST(MESSAGE_PORN_DETECTOR, preproccessing) {

    PornTextDetector txt_detector;

    std::string msg = "сок и виски под окном";
    std::string res = txt_detector.preproccesing(msg);
    EXPECT_TRUE(res != "сок виски окном");
}

TEST(MESSAGE_PORN_DETECTOR, text_replace) {

    PornTextDetector txt_detector;

    std::string msg = "порно смотреть онлайн";

    std::string res = txt_detector.text_replace(msg);
    EXPECT_TRUE(res == "***** ******** ******");
}

// TEST(MESSAGE_PORN_DETECTOR, forward) {

//     PornTextDetector txt_detector;

//     std::string msg = "порно смотреть онлайн";
//     std::string res = txt_detector.preproccesing(msg);
//     auto probability = txt_detector.forward(res);
//     EXPECT_GT(probability.porn, 0.5);
//     txt_detector.text_replace(msg);
// }

int main(int argc, char *argv[]) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
