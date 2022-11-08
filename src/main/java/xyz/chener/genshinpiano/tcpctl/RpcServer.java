package xyz.chener.genshinpiano.tcpctl;

import io.netty.bootstrap.ServerBootstrap;
import io.netty.channel.*;
import io.netty.channel.nio.NioEventLoopGroup;
import io.netty.channel.socket.SocketChannel;
import io.netty.channel.socket.nio.NioServerSocketChannel;
import io.netty.handler.codec.serialization.ObjectDecoder;
import io.netty.handler.codec.serialization.ObjectEncoder;


public class RpcServer {

    public static final int port = 35963;

    public void run()
    {
        NioEventLoopGroup bossGroup = new NioEventLoopGroup(1);
        NioEventLoopGroup workGroup = new NioEventLoopGroup(10);

        ServerBootstrap bootstrap = new ServerBootstrap()
                .group(bossGroup, workGroup)
                .channel(NioServerSocketChannel.class)
                .childHandler(new ChannelInitializer<SocketChannel>() {
                    @Override
                    protected void initChannel( SocketChannel socketChannel) throws Exception {
                        ChannelPipeline p = socketChannel.pipeline();
                        p.addLast(new ObjectDecoder(className -> MsInfo.class));
                        p.addLast(new ObjectEncoder());
                        p.addLast(new ChannelInboundHandlerAdapter(){
                            @Override
                            public void channelActive( ChannelHandlerContext ctx) throws Exception {
                            }

                            @Override
                            public void channelInactive( ChannelHandlerContext ctx) throws Exception {
                            }

                            @Override
                            public void channelRead( ChannelHandlerContext ctx, Object msg) throws Exception {
                                if (msg instanceof MsInfo d)
                                {
                                    RpcCtl.getInstance().postMusic(d);
                                }
                            }

                            @Override
                            public void exceptionCaught(ChannelHandlerContext ctx, Throwable cause) throws Exception {
                            }
                        });
                    }
                })
                .localAddress(port)
                .childOption(ChannelOption.TCP_NODELAY, true)
                .option(ChannelOption.SO_BACKLOG, 1024)
                .childOption(ChannelOption.SO_KEEPALIVE, true);

        try {
            ChannelFuture future = bootstrap.bind(port).sync();
            future.channel().closeFuture().sync();
        } catch (InterruptedException e) {
            throw new RuntimeException(e);
        }
    }

}
